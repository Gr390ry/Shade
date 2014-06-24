
#include "Transform.h"

namespace Component {

	ImplementRTTI(Transform, IComponent);

	void Transform::Start()
	{
		parent = nullptr;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matTransform);
		D3DXMatrixIdentity(&matScaling);
		D3DXMatrixIdentity(&matRotator);

		ResetComponent();
	}
	void Transform::ResetComponent()
	{
		vPosition = Vector3(0, 0, 0);
		vRollPitchYaw = Vector3(0, 0, 0);
		vScale = Vector3(1, 1, 1);
		Update(0);
	}
	void Transform::Update(float pDelta)
	{
		D3DXMatrixTranslation(&matTransform, vPosition.x, vPosition.y, vPosition.z);
		D3DXMatrixScaling(&matScaling, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationYawPitchRoll(&matRotator, vRollPitchYaw.y, vRollPitchYaw.x, vRollPitchYaw.z);

		matWorld = matScaling * matRotator * matTransform;

		if (parent)
		{
			matWorld = parent->matWorld;
			D3DXMatrixMultiply(&matWorld, &parent->matWorld, &matWorld);
		}
	}

	//set
	void Transform::SetParent(Transform* _parent)
	{
		if (this == _parent) return;
		parent = _parent;
	}
	void Transform::SetPosition(const Vector3 _position)
	{
		vPosition = _position;
	}
	void Transform::SetRollPitchYaw(const Vector3 _rollpitchyaw)
	{
		vRollPitchYaw = _rollpitchyaw;
	}
	void Transform::SetScale(const Vector3 _scale)
	{
		vScale = _scale;
	}
	//get
	Transform* Transform::GetParent() const
	{
		return parent;
	}
	const Matrix4x4& Transform::GetWorldMatrix()
	{
		return matWorld;
	}
	/*inline const Matrix4x4& Transform::GetLocalMatrix()
	{
		return matLocal;
	}*/
	const Vector3& Transform::GetPosition()
	{
		return vPosition;
	}
	const Vector3& Transform::GetRollPitchYaw()
	{
		return vRollPitchYaw;
	}
	const Vector3& Transform::GetScale()
	{
		return vScale;
	}
}