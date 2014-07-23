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
	}

	void Render::Release()
	{
		RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}
	
	void Render::Draw()
	{
		UINT stride = sizeof(GENERIC::Vertex);
		UINT offset = 0;

		ID3D11DeviceContext* context = RenderDevice::Get()->GetContext();

		context->IASetInputLayout(_inputLayout);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
		context->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);

		D3DX11_TECHNIQUE_DESC	techDesc;
		_tech->GetDesc(&techDesc);

		XMMATRIX world	= pOwner->GetComponent<Transform>()->GetWorldMatrix();
		XMMATRIX vp		= General::Get()->GetMainCamera()->GetViewProjectionMatrix();
		
		Effect::BasicEffect::Get()->SetWorldMatrix(reinterpret_cast<float*>(&world));
		Effect::BasicEffect::Get()->SetViewProjectionMatrix(reinterpret_cast<float*>(&vp));

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			_tech->GetPassByIndex(i)->Apply(0, context);
			context->DrawIndexed(_meshData->GetNumIndices(), 0, 0);
		}
	}

	void Render::SetMeshData(IMesh* mesh)
	{
		if (mesh == nullptr) return;

		_meshData = mesh;

		pVB = _meshData->GetVB();
		pIB = _meshData->GetIB();
		_inputLayout	= Effect::BasicEffect::Get()->GetLayout();
		_tech			= Effect::BasicEffect::Get()->GetTechique();

		//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//};
		//D3DX11_PASS_DESC passDesc;

		//_tech = Effect::BasicEffect::Get()->GetFx()->GetTechniqueByName("BasicTech");//GetTechniqueByIndex(0);

		//assert(_tech != nullptr);

		//_tech->GetPassByIndex(0)->GetDesc(&passDesc);

		//ID3D11Device* device = RenderDevice::Get()->GetDevice();
		//if (FAILED(device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_inputLayout)))
		//{
		//	assert(false);
		//}

		//CreateVertexBuffer();
		//CreateIndexBuffer();
	}

	void Render::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC vbd;
		vbd.Usage				= D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth			= sizeof(GENERIC::Vertex) * _meshData->GetNumVertices();
		vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags		= 0;
		vbd.MiscFlags			= 0;
		vbd.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = _meshData->GetVertices();

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
		ibd.ByteWidth			= sizeof(UINT) * _meshData->GetNumIndices();
		ibd.BindFlags			= D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags		= 0;
		ibd.MiscFlags			= 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = _meshData->GetIndices();//&mIndices[0];

		if (FAILED(RenderDevice::Get()->GetDevice()->CreateBuffer(&ibd, &initData, &pIB)))
		{
			assert(NULL);
		}
	}
};