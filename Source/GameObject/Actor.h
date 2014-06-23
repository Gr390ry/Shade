#pragma once
#include "IGameObject.h"
//#include <vector>

namespace Component {
	class Render;
	class Transform;
}

namespace GameObject {
	
	class Actor : public IGameObject
	{
	public:
		Actor();
		~Actor();

		void Initialize();
		void Update(float);
		void Release();
		
		void AddComponent(Component::IComponent*);
		const bool GetActvate();
		Component::Transform* GetTransform();
		const Component::Render* GetRender();

	private:
		Component::Render* pRender;
		Component::Transform* pTransform;
		//std::vector<Component::IComponent*> listComponents;
	};

};