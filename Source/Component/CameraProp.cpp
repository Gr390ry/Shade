#include "CameraProp.h"	
#include "Transform.h"
#include "../GameObject/IGameObject.h"

namespace Component {
	ImplementRTTI(CameraProp, IComponent);

	CameraProp::CameraProp()
	{
	}
	CameraProp::~CameraProp()
	{
	}

	void CameraProp::Initialize()
	{
		ResetComponent();
	}

	void CameraProp::Release()
	{
	}

	void CameraProp::ResetComponent()
	{
		mLookAt = Vector3(0, 0, 0);
		D3DXMatrixIdentity(&mViewMatrix);
		D3DXMatrixIdentity(&mProjectionMatrix);
	}

	void CameraProp::Update(float pDelta)
	{
		if (pOwner == nullptr || !pOwner->GetActvate()) return;

		Transform* pTransform = pOwner->GetComponent<Transform>();
		Vector3 vEye;

		if (pTransform)
			vEye = pTransform->GetPosition();
		else
			vEye = Vector3();

		D3DXMatrixLookAtLH(&mViewMatrix, &vEye, &mLookAt, &Vector3(0, 1, 0));
		D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, GENERIC::FOV, GENERIC::ASPECT_RATIO, GENERIC::nearPlane, GENERIC::farPlane);
	}

}; /*Component*/