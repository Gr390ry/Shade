#pragma once
#include "IGameObject.h"
#include "../ShadeEngine.h"
#include "../ISingleton.h"

namespace Component {
	class Transform;
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

	private:
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;
	};

}; /*GameObject*/