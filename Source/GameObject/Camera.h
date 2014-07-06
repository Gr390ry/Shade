#pragma once
#include "IGameObject.h"
#include "../ShadeEngine.h"

namespace Component {
	class Transform;
	class CameraProp;
}

namespace GameObject {

	class Camera : public IGameObject
	{
	public:
		Camera();
		~Camera();

		void				Initialize()		override;
		void				Update(float)		override;
		void				Release()			override;
		const bool			GetActvate()		override { return mbActivated; }
		void				AttachTo(const IGameObject*);		
		const XMMATRIX&		GetViewMarix();
		const XMMATRIX&		GetProjectionMatrix();
		void				SetLookAt(const XMFLOAT3&);
		void				SetPosition(const XMFLOAT3&);

	private:
		Component::CameraProp* pProperty;
		Component::Transform* pTransform;
	};

}; /*GameObject*/