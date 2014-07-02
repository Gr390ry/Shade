#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../GameObject/Camera.h"
#include "../Management/General.h"
#include "../Management/RenderDevice.h"
#include "../Mesh/IMesh.h"
#include "../Mesh/StaticMesh.h"

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
		pMeshData = new StaticMesh;
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
	
	void Render::DrawFbxMesh(ID3DX11EffectTechnique* technique)
	{
		XMMATRIX world = pOwner->GetComponent<Transform>()->GetWorldMatrix();
		XMMATRIX view = General::Get()->GetMainCamera()->GetViewMarix();
		XMMATRIX projection = General::Get()->GetMainCamera()->GetProjectionMatrix();

		XMMATRIX vp = XMMatrixMultiply(view, projection);
		ID3DX11EffectMatrixVariable* fxWorld = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gWorld")->AsMatrix();
		ID3DX11EffectMatrixVariable* fxViewProjection = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gViewProjection")->AsMatrix();
		ID3DX11EffectVectorVariable* fxWorldLightPosition = RenderDevice::Get()->GetShaderDEMO()->GetVariableByName("gWorldLightPosition")->AsVector();
		
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
			technique->GetPassByIndex(i)->Apply(0, RenderDevice::Get()->GetContext());
			//RenderDevice::Get()->GetContext()->DrawIndexed(pMeshData->GetNumIndices(), 0, 0);
			RenderDevice::Get()->GetContext()->Draw(pMeshData->GetNumVertices(), 0);
		}
	}

	void Render::LoadFbxModel(const char* filepath)
	{
		if (!pMeshData) return;

		pMeshData->Initialize(filepath);
		CreateVertexBuffer();
		CreateIndexBuffer();
		//pMeshData->Release();
	}

	void Render::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(IMesh::Vertex) * pMeshData->GetNumVertices();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pMeshData->GetVertices();//&mVertices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &pVB)))
		{
			assert(NULL);
		}
	}

	void Render::CreateIndexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(int) * pMeshData->GetNumIndices();
		vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pMeshData->GetIndices();//&mIndices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&vbd, &initData, &pIB)))
		{
			assert(NULL);
		}
	}
};