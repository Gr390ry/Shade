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
		void Initialize();
		void Release() {};
		void Update(float);

		void Translate(const XMFLOAT3&);
		
		const char* GetID() { return "Transform"; }
		const EFamily::_ GetFamilyID() { return EFamily::System; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }

		//set
		void SetParent(Transform*);
		void SetPosition(const XMFLOAT3);
		void SetRollPitchYaw(const XMFLOAT3);
		void SetScale(const XMFLOAT3);

		//get
		Transform* GetParent() const;
		const XMMATRIX& GetWorldMatrix();
		const XMMATRIX& GetLocalMatrix();
		const XMFLOAT3& GetPosition();
		const XMFLOAT3& GetRollPitchYaw();
		const XMFLOAT3& GetScale();

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
		XMMATRIX matWorld;
		//Matrix4x4 matLocal;
		XMMATRIX matTransform;
		XMMATRIX matRotator;
		XMMATRIX matScaling;

		//vector
		XMFLOAT3 vPosition;
		XMFLOAT3 vRollPitchYaw;
		XMFLOAT3 vScale;

		//parent(world)
		Transform* parent;
	};
}; /*Component*/