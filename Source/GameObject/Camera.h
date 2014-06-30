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
		const XMMATRIX& GetViewMarix();
		const XMMATRIX& GetProjectionMatrix();
		void SetLookAt(const XMFLOAT3&);
		void SetPosition(const XMFLOAT3&);

	private:
		Component::CameraProp* pProperty;
		Component::Transform* pTransform;
	};

}; /*GameObject*/