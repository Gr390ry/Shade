#include "Render.h"
#include "../RenderDevice.h"

namespace Component
{
	Render::Render() : pMesh(nullptr), pShader(nullptr), pDiffuseMap(nullptr), pNormalMap(nullptr), pSpecularMap(nullptr)
	{
	}

	Render::~Render()
	{
	}

	void Render::Start()
	{
		RenderDevice::Get()->AddListener(this);
	}

	void Render::End()
	{
		//RenderDevice::Get()->RemoveListener(this);
	}

	void Render::ResetComponent()
	{
	}

	void Render::Update(float pDelta)
	{
	}

	void Render::DrawOnScreen(const LPD3DXEFFECT _shader)
	{
		_shader->SetMatrix("WorldMatrix", pWorldMatrix);
		_shader->CommitChanges();
		pMesh->DrawSubset(0);
	}
	void Render::SetMesh(const LPD3DXMESH _mesh)
	{
		pMesh = _mesh;
	}
	void Render::SetWorldMatrix(const Matrix4x4* worldMatrix)
	{
		pWorldMatrix = const_cast<Matrix4x4*>(worldMatrix);
	}
};