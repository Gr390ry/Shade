#pragma once
#include "IGameObject.h"
#include "../ShadeEngine.h"

namespace Component {
	class Transform;
	class CameraProp;
}

namespace GameObject {

	class Actor;
	class Camera : public IGameObject
	{
	public:
		Camera();
		~Camera();

		void				Initialize()		override;
		void				Update(float)		override;
		void				Release()			override;
		const bool			GetActvate()		override { return _IsActivated; }
		void				AttachTo(const IGameObject*);		
		const XMMATRIX&		GetViewMarix();
		const XMMATRIX&		GetProjectionMatrix();
		const XMMATRIX&		GetViewProjectionMatrix();
		void				SetLookAt(const XMFLOAT3&);
		void				SetPosition(const XMFLOAT3&);
		void				SetTarget(const Actor*);

	private:
		Component::CameraProp*	_cameraProp;
		Component::Transform*	_transform;		
		Actor*					_targetActor;
	};

}; /*GameObject*/