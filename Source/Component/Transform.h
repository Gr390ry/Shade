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
		void				ResetComponent()	override;
		void				Initialize()		override;
		void				Release()			override {};
		void				Update(float)		override;
		const char*			GetID()				override { return "Transform"; }
		const EFamily::_	GetFamilyID()		override { return EFamily::System; }
		void				SetOwner(GameObject::IGameObject* owner) override { pOwner = owner; }
		void				Translate(const XMFLOAT3&);
		//set
		void				SetParent(Transform*);
		void				SetPosition(const XMFLOAT3);
		void				SetRollPitchYaw(const XMFLOAT3);
		void				SetScale(const XMFLOAT3);
		//get
		Transform*			GetParent() const;
		const XMMATRIX&		GetWorldMatrix();
		const XMMATRIX&		GetLocalMatrix();
		const XMFLOAT3&		GetPosition();
		const XMFLOAT3&		GetRollPitchYaw();
		const XMFLOAT3&		GetScale();

		//Matrix16�� �� �޸𸮿� �Ҵ� �ɶ� 16����Ʈ�� �Ҵ�Ǳ⋚���� ������ ����ų �� �ִ�.
		//�׷���! new �����ڸ� �����ε� �ؼ� ������ �������ش�! 16����Ʈ�� �Ҵ��ϰԲ�!
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