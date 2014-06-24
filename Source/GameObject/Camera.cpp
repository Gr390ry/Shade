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
		AddComponent<Component::Transform>();
		AddComponent<Component::CameraProp>();
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

};