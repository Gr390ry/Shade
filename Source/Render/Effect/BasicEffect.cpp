#include "BasicEffect.h"
#include "../../Management/RenderDevice.h"

namespace Render { namespace Effect {

	void BasicEffect::SetInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		D3DX11_PASS_DESC passDesc;

		_technique = object->GetTechniqueByIndex(0);
		_technique->GetPassByIndex(0)->GetDesc(&passDesc);


		ID3D11Device* device = RenderDevice::Get()->GetDevice();

		assert(device != nullptr);

		device->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_inputLayout);

		assert(_inputLayout != nullptr);
	}

	void BasicEffect::Initialize()
	{
		assert(object != nullptr);

		SetInputLayout();

		_world = object->GetVariableByName("gWorld")->AsMatrix();
		_viewProjection = object->GetVariableByName("gViewProj")->AsMatrix();
		_lightDirection = object->GetVariableByName("gLightDirection")->AsVector();
	}

	void BasicEffect::Release()
	{
	}

	void BasicEffect::SetWorldMatrix(XMMATRIX& world)
	{
		//assert(world != nullptr);
		_world->SetMatrix(reinterpret_cast<float*>(&world));
	}

	void BasicEffect::SetViewProjectionMatrix(XMMATRIX& viewProjection)
	{
		//assert(viewProjection != nullptr);
		_viewProjection->SetMatrix(reinterpret_cast<float*>(&viewProjection));
	}

	void BasicEffect::SetLightDirection(XMVECTOR& lightDirection)
	{
		//assert(lightDirection != nullptr);
		_lightDirection->SetFloatVector(reinterpret_cast<float*>(&lightDirection));
	}

} /*Effect*/ } /*Render*/