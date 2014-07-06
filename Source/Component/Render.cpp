#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../GameObject/Camera.h"
#include "../Management/General.h"
#include "../Management/RenderDevice.h"
#include "../Mesh/IMesh.h"
#include "../Mesh/StaticMesh.h"
#include "../Mesh/BoxMesh.h"

namespace Component
{
	ImplementRTTI(Render, IRenderable);

	Render::Render()
	{
	}

	Render::~Render()
	{
	}

	void Render::Initialize()
	{
		RenderDevice::Get()->AddListener(this);
		pMeshData = new BoxMesh;

		if (FAILED(D3DX11CreateShaderResourceViewFromFile(RenderDevice::Get()->GetDevice(), 
														  "Contents/Texture/DiffuseTest.dds", 
														  0, 0, &pDiffuseMap, 0)))
		{
			Console::Get()->print("Texture Load Failed");
		}
	}

	void Render::Release()
	{
		SAFE_RELEASE(pVB);
		SAFE_RELEASE(pIB);
		RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}
	
	void Render::RendMesh(ID3DX11EffectTechnique* technique)
	{
		XMMATRIX world = pOwner->GetComponent<Transform>()->GetWorldMatrix();
		XMMATRIX view = General::Get()->GetMainCamera()->GetViewMarix();
		XMMATRIX projection = General::Get()->GetMainCamera()->GetProjectionMatrix();

		XMMATRIX vp = XMMatrixMultiply(view, projection);
		
		{
			ID3DX11EffectMatrixVariable* fxWorld = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gWorld")->AsMatrix();
			ID3DX11EffectMatrixVariable* fxViewProjection = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gViewProjection")->AsMatrix();
			ID3DX11EffectVectorVariable* fxWorldLightPosition = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gWorldLightPosition")->AsVector();
			ID3DX11EffectShaderResourceVariable* fxDiffuseMap = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gDiffuseMap")->AsShaderResource();

			fxWorld->SetMatrix(reinterpret_cast<float*>(&world));
			fxViewProjection->SetMatrix(reinterpret_cast<float*>(&vp));


			float lightPosition[] = { 500, 500, -500, 1 };

			fxWorldLightPosition->SetFloatVector(lightPosition);

			UINT stride = sizeof(IMesh::Vertex);
			UINT offset = 0;
			RenderDevice::Get()->GetContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
			RenderDevice::Get()->GetContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

			D3DX11_TECHNIQUE_DESC techDesc;
			technique->GetDesc(&techDesc);

			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				fxDiffuseMap->SetResource(pDiffuseMap);
				technique->GetPassByIndex(i)->Apply(0, RenderDevice::Get()->GetContext());
				RenderDevice::Get()->GetContext()->DrawIndexed(pMeshData->GetNumIndices(), 0, 0);
				//RenderDevice::Get()->GetContext()->Draw(pMeshData->GetNumVertices(), 0);
			}
		}
	}

	//void Render::LoadFbxModel(const char* filepath)
	//{
	//	if (!pMeshData) return;

	//	if (!pMeshData->Initialize(filepath))
	//	{
	//		Console::Get()->print("Err>Mesh Initialize Failed");
	//		assert(NULL);
	//		return;
	//	}
	//	CreateVertexBuffer();
	//	CreateIndexBuffer();
	//	//pMeshData->Release();
	//}

	void Render::SetMeshData(IMesh* pMesh)
	{
		if (pMesh == nullptr) return;

		pMeshData = pMesh;
		CreateVertexBuffer();
		CreateIndexBuffer();
	}

	void Render::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage				= D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth			= sizeof(IMesh::Vertex) * pMeshData->GetNumVertices();
		vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags		= 0;
		vbd.MiscFlags			= 0;
		vbd.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pMeshData->GetVertices();

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &pVB)))
		{
			assert(NULL);
			Console::Get()->print("Err>Create Vertex Buffer Failed!\n");
			return;
		}
	}

	void Render::CreateIndexBuffer()
	{
		D3D11_BUFFER_DESC ibd;
		ibd.Usage				= D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth			= sizeof(int) * pMeshData->GetNumIndices();
		ibd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags		= 0;
		ibd.MiscFlags			= 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pMeshData->GetIndices();//&mIndices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&ibd, &initData, &pIB)))
		{
			assert(NULL);
		}
	}
};