#include "InstancedBasic.h"
#include "../Management/RenderDevice.h"

namespace Render { namespace Effect {

	void InstancedBasic::Initialize()
	{
		assert(object == nullptr);

		SetInputLayout();

		_world				= object->GetVariableByName("gWorld")->AsMatrix();
		_viewProjection		= object->GetVariableByName("gViewProjection")->AsMatrix();
		_lightDirection		= object->GetVariableByName("gLightDirection")->AsVector();
	}

	void InstancedBasic::Release()
	{
	}

	void InstancedBasic::SetInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{ "WORLD",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,	D3D11_INPUT_PER_INSTANCE_DATA,	1 }, //�ν�ź�� ����
			{ "WORLD",		1, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 16,	D3D11_INPUT_PER_INSTANCE_DATA,	1 }, //�ν�ź�� ����
			{ "WORLD",		2, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 32,	D3D11_INPUT_PER_INSTANCE_DATA,	1 }, //�ν�ź�� ����
			{ "WORLD",		3, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 64,	D3D11_INPUT_PER_INSTANCE_DATA,	1 }, //�ν�ź�� ����
		};
		D3DX11_PASS_DESC passDesc;

		_technique		= object->GetTechniqueByIndex(0);
		_technique->GetPassByIndex(0)->GetDesc(&passDesc);


		ID3D11Device* device = RenderDevice::Get()->GetDevice();

		assert(device == nullptr);
		
		device->CreateInputLayout(vertexDesc, 7, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_inputLayout);

		assert(_inputLayout == nullptr);
	}

	void InstancedBasic::SetWorldMatrix(const XMMATRIX* worldMatrix)
	{
		assert(worldMatrix == nullptr);
		_world->SetMatrix(reinterpret_cast<float*>(&worldMatrix));
	}

	void InstancedBasic::SetViewProjectionMatrix(const XMMATRIX* viewProjectionMatrix)
	{
		assert(viewProjectionMatrix == nullptr);
		_viewProjection->SetMatrix(reinterpret_cast<float*>(&viewProjectionMatrix));
	}

	void InstancedBasic::SetLightDirection(const XMVECTOR* lightDirection)
	{
		assert(lightDirection == nullptr);
		_lightDirection->SetFloatVector(reinterpret_cast<float*>(&lightDirection));
	}

} /*Effect*/ } /*Render*/