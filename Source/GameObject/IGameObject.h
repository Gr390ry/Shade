#pragma once
#include "RequireComponents.h"

namespace Component {
	class IComponent;
	//class Transform;
};

namespace GameObject {

	class IGameObject
	{
	public:
		virtual ~IGameObject() {}
		virtual void Initialize() = 0;
		virtual void Update(float) = 0;
		virtual void Release() = 0;
		virtual const bool GetActvate() = 0;
		virtual void AddComponent(Component::IComponent*) = 0;

	protected:		
		bool mbActivated;
	};

}