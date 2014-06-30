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
		mLookAt				= XMFLOAT3(0, 0, 0);
		mViewMatrix			= XMMatrixIdentity();
		mProjectionMatrix	= XMMatrixIdentity();
	}

	void CameraProp::Update(float pDelta)
	{
		if (pOwner == nullptr || !pOwner->GetActvate()) return;

		Transform* pTransform = pOwner->GetComponent<Transform>();
		XMFLOAT3 eye;

		if (pTransform)
			eye = pTransform->GetPosition();
		else
			eye = XMFLOAT3(0, 0, 0);

		/*D3DXMatrixLookAtLH(&mViewMatrix, &vEye, &mLookAt, &Vector3(0, 1, 0));
		D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, GENERIC::FOV, GENERIC::ASPECT_RATIO, GENERIC::nearPlane, GENERIC::farPlane);*/

		XMVECTOR vEye	= XMVectorSet(eye.x, eye.y, eye.z, 0);
		XMVECTOR vFocus = XMVectorSet(mLookAt.x, mLookAt.y, mLookAt.z, 0);
		XMVECTOR vUp	= XMVectorSet(0, 1, 0, 0);
		mViewMatrix		= XMMatrixLookAtLH(vEye, vFocus, vUp);
		mProjectionMatrix = XMMatrixPerspectiveFovLH(GENERIC::FOV, GENERIC::ASPECT_RATIO, GENERIC::nearPlane, GENERIC::farPlane);
	}

}; /*Component*/