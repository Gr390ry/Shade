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

		void SetLookAt(XMFLOAT3 vLook) { mLookAt = vLook; }
		const XMFLOAT3& GetLookAt() { return mLookAt; }
		const XMMATRIX& GetViewMatrix() { return mViewMatrix; }
		const XMMATRIX& GetProjectionMatrix() { return mProjectionMatrix; }

		static void* operator new (size_t size)
		{
			void* p = _aligned_malloc(sizeof(CameraProp), 16);
			return p;
		}
		static void operator delete (void* p)
		{
			static_cast<CameraProp*>(p)->~CameraProp();
			_aligned_free(p);
			p = nullptr;
		}

	private:
		XMFLOAT3 mLookAt;
		XMMATRIX mViewMatrix;
		XMMATRIX mProjectionMatrix;
	};

}; /*Component*/