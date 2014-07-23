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
		_transform	= AddComponent<Component::Transform>();
		_cameraProp	= AddComponent<Component::CameraProp>();

		_IsActivated = true;
		_targetActor	= nullptr;
	}

	void Camera::Update(float deltaTime)
	{
		UPDATE_COMPONENTS(deltaTime);
	}

	void Camera::Release()
	{
		RemoveAllComponents();
	}
	
	const XMMATRIX& Camera::GetViewMarix()
	{
		assert(_cameraProp != nullptr);
		return _cameraProp->GetViewMatrix();
	}
	const XMMATRIX& Camera::GetProjectionMatrix()
	{
		assert(_cameraProp != nullptr);
		return _cameraProp->GetProjectionMatrix();
	}
	const XMMATRIX& Camera::GetViewProjectionMatrix()
	{
		assert(_cameraProp != nullptr);
		return XMMatrixMultiply(_cameraProp->GetViewMatrix(), _cameraProp->GetProjectionMatrix());
	}
	void Camera::SetLookAt(const XMFLOAT3& look)
	{
		assert(_cameraProp != nullptr);
		_cameraProp->SetLookAt(look);
	}
	void Camera::SetPosition(const XMFLOAT3& position)
	{
		assert(_transform != nullptr);
		_transform->SetPosition(position);
	}
};