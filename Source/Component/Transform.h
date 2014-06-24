#pragma once

#include "IComponent.h"
#include "../ShadeEngine.h"
#include <new>

namespace Component {

	class Transform : public IComponent
	{
		DeclareRTTI(Transform);
	public:
		Transform() {}
		virtual ~Transform() {}

		//overrode
		void ResetComponent();
		void Start();
		void End() {};
		void Update(float);
		
		const char* GetID() { return "Transform"; }
		const EFamily::_ GetFamilyID() { return EFamily::System; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }

		//set
		void SetParent(Transform*);
		void SetPosition(const Vector3);
		void SetRollPitchYaw(const Vector3);
		void SetScale(const Vector3);

		//get
		Transform* GetParent() const;
		const Matrix4x4& GetWorldMatrix();
		const Matrix4x4& GetLocalMatrix();
		const Vector3& GetPosition();
		const Vector3& GetRollPitchYaw();
		const Vector3& GetScale();

		//Matrix16이 힙 메모리에 할당 될때 16바이트로 할당되기떄문에 문제를 일으킬 수 있다.
		//그래서! new 연산자를 오버로딩 해서 새로이 정의해준다! 16바이트로 할당하게끔!
		static void* operator new (size_t size) 
		{
			void* p = _aligned_malloc(sizeof(Transform), 16);
			return p;
		}
		static void operator delete (void* p)
		{
			static_cast<Transform*>(p)->~Transform();
			_aligned_free(p);
			p = nullptr;
		}

	private:
		//matrix
		Matrix4x4 matWorld;
		//Matrix4x4 matLocal;
		Matrix4x4 matTransform;
		Matrix4x4 matRotator;
		Matrix4x4 matScaling;

		//vector
		Vector3 vPosition;
		Vector3 vRollPitchYaw;
		Vector3 vScale;

		//parent(world)
		Transform* parent;
	};
}; /*Component*/