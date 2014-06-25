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

		void Initialize();
		void Update(float);
		void Release();

		void AttachTo(const IGameObject*);

		const bool GetActvate() { return mbActivated; }
		const Matrix4x4& GetViewMarix();
		const Matrix4x4& GetProjectionMatrix();
		void SetLookAt(const Vector3&);
		void SetPosition(const Vector3&);

	private:
		Component::CameraProp* pProperty;
		Component::Transform* pTransform;
	};

}; /*GameObject*/