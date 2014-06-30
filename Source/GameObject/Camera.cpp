#include "Camera.h"
#include "../Component/Transform.h"
#include "../Component/CameraProp.h"


namespace GameObject {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{		
	}

	void Camera::Initialize()
	{
		mbActivated = true;
		pTransform	= AddComponent<Component::Transform>();
		pProperty	= AddComponent<Component::CameraProp>();		
	}

	void Camera::Update(float pDelta)
	{
		ITER_COMPONENT iter = ContainComponents.begin();
		int nSize = ContainComponents.size();
		concurrency::parallel_for(0, nSize, [&](int n)
		{
			iter->second->Update(pDelta);
			++iter;
		});
	}

	void Camera::Release()
	{
		RemoveAllComponents();
	}
	
	const XMMATRIX& Camera::GetViewMarix()
	{
		return pProperty->GetViewMatrix();
	}
	const XMMATRIX& Camera::GetProjectionMatrix()
	{
		return pProperty->GetProjectionMatrix();
	}
	void Camera::SetLookAt(const XMFLOAT3& look)
	{
		pProperty->SetLookAt(look);
	}
	void Camera::SetPosition(const XMFLOAT3& position)
	{
		pTransform->SetPosition(position);
	}
};