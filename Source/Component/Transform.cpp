
#include "Transform.h"

namespace Component {

	ImplementRTTI(Transform, IComponent);

	void Transform::Initialize()
	{
		parent = nullptr;

		ResetComponent();
	}
	void Transform::ResetComponent()
	{
		vPosition		= XMFLOAT3(0, 0, 0);
		vRollPitchYaw	= XMFLOAT3(0, 0, 0);
		vScale			= XMFLOAT3(1, 1, 1);
		matWorld		= XMMatrixIdentity();
		matTransform	= XMMatrixIdentity();
		matScaling		= XMMatrixIdentity();
		matRotator		= XMMatrixIdentity();
	}
	void Transform::Update(float pDelta)
	{
		matScaling		= XMMatrixScaling(vScale.x, vScale.y, vScale.z);
		matRotator		= XMMatrixRotationRollPitchYaw(vRollPitchYaw.x, vRollPitchYaw.y, vRollPitchYaw.z);
		matTransform	= XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);	

		matWorld		= matScaling * matRotator * matTransform;

		if (parent)
		{
			matWorld	= XMMatrixMultiply(matWorld, parent->matWorld);
		}
	}

	void Transform::Translate(const XMFLOAT3& vVelocity)
	{
		vPosition.x += vVelocity.x;
		vPosition.y += vVelocity.y;
		vPosition.z += vVelocity.z;
	}

	//set
	void Transform::SetParent(Transform* _parent)
	{
		if (this == _parent) return;
		parent = _parent;
	}
	void Transform::SetPosition(const XMFLOAT3 _position)
	{
		vPosition = _position;
	}
	void Transform::SetRollPitchYaw(const XMFLOAT3 _rollpitchyaw)
	{
		vRollPitchYaw = _rollpitchyaw;
	}
	void Transform::SetScale(const XMFLOAT3 _scale)
	{
		vScale = _scale;
	}
	//get
	Transform* Transform::GetParent() const
	{
		return parent;
	}
	const XMMATRIX& Transform::GetWorldMatrix()
	{
		return matWorld;
	}
	const XMFLOAT3& Transform::GetPosition()
	{
		return vPosition;
	}
	const XMFLOAT3& Transform::GetRollPitchYaw()
	{
		return vRollPitchYaw;
	}
	const XMFLOAT3& Transform::GetScale()
	{
		return vScale;
	}
}