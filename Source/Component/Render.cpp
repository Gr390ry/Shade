#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../GameObject/Camera.h"
#include "../Management/General.h"
#include "../Management/RenderDevice.h"
#include "../Render/IMesh.h"
#include "../Render/Mesh/StaticMesh.h"
#include "../Render/Mesh/BoxMesh.h"
#include "../Render/Effect/BasicEffect.h"



namespace Component
{
	using namespace Render;

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
		//pMeshData = new BoxMesh;
		//if (FAILED(D3DX11CreateShaderResourceViewFromFile(RenderDevice::Get()->GetDevice(), 
		//												  "Contents/Texture/images.jpg", 
		//												  0, 0, &pDiffuseMap, 0)))
		//{
		//	Console::Get()->print("Texture Load Failed");
		//}
	}

	void Render::Release()
	{
		//SAFE_RELEASE(pVB);
		//SAFE_RELEASE(pIB);
		RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}
	
	void Render::RendMesh(ID3DX11EffectTechnique* technique = nullptr)
	{
		XMMATRIX world			= pOwner->GetComponent<Transform>()->GetWorldMatrix();
		XMMATRIX view			= General::Get()->GetMainCamera()->GetViewMarix();
		XMMATRIX projection		= General::Get()->GetMainCamera()->GetProjectionMatrix();

		XMMATRIX vp = XMMatrixMultiply(view, projection);
		
		{
			UINT stride = sizeof(GENERIC::Vertex);
			UINT offset = 0;

			ID3D11DeviceContext* context = RenderDevice::Get()->GetContext();

			context->IASetInputLayout(Effect::BasicEffect::Get()->GetLayout());
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			context->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
			context->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

			D3DX11_TECHNIQUE_DESC techDesc;
			LPD3D11EFFECTTECHNIQUE technique = Effect::BasicEffect::Get()->GetTechique();
			//technique->GetDesc(&techDesc);
			technique->GetDesc(&techDesc);
			Effect::BasicEffect::Get()->SetWorldMatrix(world);
			Effect::BasicEffect::Get()->SetViewProjectionMatrix(vp);
			Effect::BasicEffect::Get()->SetLightDirection(XMLoadFloat4(&GENERIC::worldLightPosition));

			
			
			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				//fxDiffuseMap->SetResource(pDiffuseMap);
				technique->GetPassByIndex(i)->Apply(0, context);
				context->DrawIndexed(meshData->GetNumIndices(), 0, 0);
				//RenderDevice::Get()->GetContext()->Draw(pMeshData->GetNumVertices(), 0);
			}
		}
	}

	void Render::SetMeshData(IMesh* mesh)
	{
		if (mesh == nullptr) return;

		meshData = mesh;

		pVB		= meshData->GetVB();
		pIB		= meshData->GetIB();

		/*CreateVertexBuffer();
		CreateIndexBuffer();*/
	}

	void Render::CreateVertexBuffer()
	{
		/*D3D11_BUFFER_DESC vbd;
		vbd.Usage				= D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth			= sizeof(GENERIC::Vertex) * pMeshData->GetNumVertices();
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
		}*/
	}

	void Render::CreateIndexBuffer()
	{
		//D3D11_BUFFER_DESC ibd;
		//ibd.Usage				= D3D11_USAGE_IMMUTABLE;
		//ibd.ByteWidth			= sizeof(int) * pMeshData->GetNumIndices();
		//ibd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
		//ibd.CPUAccessFlags		= 0;
		//ibd.MiscFlags			= 0;
		//ibd.StructureByteStride = 0;

		//D3D11_SUBRESOURCE_DATA initData;
		//initData.pSysMem = pMeshData->GetIndices();//&mIndices[0];

		//if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&ibd, &initData, &pIB)))
		//{
		//	assert(NULL);
		//}
	}
};