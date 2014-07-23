#include "BasicEffect.h"
#include "../../Management/RenderDevice.h"

namespace Render { namespace Effect {

	void BasicEffect::SetInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			/*{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },*/
		};
		D3DX11_PASS_DESC passDesc;

		_technique = _fx->GetTechniqueByIndex(0);
		_technique->GetPassByIndex(0)->GetDesc(&passDesc);

		ID3D11Device* device = RenderDevice::Get()->GetDevice();

		assert(device != nullptr);

		//device->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_inputLayout);
		device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_inputLayout);

		assert(_inputLayout != nullptr);
	}

	void BasicEffect::Initialize()
	{
		assert(_fx != nullptr);

		SetInputLayout();

		_world			= _fx->GetVariableByName("gWorld")->AsMatrix();
		_viewProjection = _fx->GetVariableByName("gViewProjection")->AsMatrix();
		_lightDirection = _fx->GetVariableByName("gLightDirection")->AsVector();
	}

	void BasicEffect::Release()
	{
	}

	void BasicEffect::SetWorldMatrix(const float* world)
	{
		assert(world != nullptr);
		_world->SetMatrix(world);
	}

	void BasicEffect::SetViewProjectionMatrix(const float* viewProjection)
	{
		assert(viewProjection != nullptr);
		_viewProjection->SetMatrix(viewProjection);
	}

	void BasicEffect::SetLightDirection(const float* lightDirection)
	{
		assert(lightDirection != nullptr);
		_lightDirection->SetFloatVector(lightDirection);
	}

} /*Effect*/ } /*Render*/