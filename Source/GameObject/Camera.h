#pragma once
#include "IGameObject.h"
#include "../ISingleton.h"

namespace Component {
	class Transform;
}

namespace GameObject {

	class Camera : public IGameObject//, ISingleton<Camera>
	{
	public:
		~Camera();

		void Initialize();
		void Update(float);
		void Release();

	private:
		Component::Transform* pTransform;
	};

}; /*GameObject*/