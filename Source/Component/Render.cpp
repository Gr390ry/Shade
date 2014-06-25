#include "Render.h"
#include "Transform.h"
#include "../GameObject/IGameObject.h"
#include "../Management/RenderDevice.h"

namespace Component
{
	ImplementRTTI(Render, IRenderable);

	Render::Render() : pMesh(nullptr), pShader(nullptr), pDiffuseMap(nullptr), pNormalMap(nullptr), pSpecularMap(nullptr)
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

	void Render::DrawOnScreen(const LPD3DXEFFECT _shader)
	{
		Transform* transform = pOwner->GetComponent<Transform>();

		if (transform)
		{
			_shader->SetMatrix("WorldMatrix", &transform->GetWorldMatrix());
		}		
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