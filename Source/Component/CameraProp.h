#pragma once
#include "IComponent.h"
#include "../ShadeEngine.h"

namespace Component {
	/*
		카메라에 필요한 정보만 가지고 잇는다. 
		LookAt, ViewMatrix, ProjectionMatrix
	*/
	class CameraProp : public IComponent
	{
		DeclareRTTI(CameraProp);
	public:
		CameraProp();
		~CameraProp();

		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);	

		const char* GetID() { return "Camera"; }
		const EFamily::_ GetFamilyID() { return EFamily::Render; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }

		void SetLookAt(Vector3 vLook) { mLookAt = vLook; }
		const Vector3& GetLookAt() { return mLookAt; }
		const Matrix4x4& GetViewMatrix() { return mViewMatrix; }
		const Matrix4x4& GetProjectionMatrix() { return mProjectionMatrix; }
	private:
		Vector3 mLookAt;
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;
	};

}; /*Component*/