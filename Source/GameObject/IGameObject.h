#pragma once
//#include "RequireComponents.h"
#include "../ShadeEngine.h"
//#include "../Component/IComponent.h"

namespace Component {
	class IComponent;
	//class Transform;
};

namespace GameObject {

	class IGameObject
	{
		//type defines
		typedef std::map<char*, Component::IComponent*> MAP_COMPONENT;
		typedef std::pair<char*, Component::IComponent*> PAIR_COMPONENT;
		typedef MAP_COMPONENT::iterator ITER_COMPONENT;

		//Functions
	public:
		virtual void Initialize() = 0;
		virtual void Update(float) = 0;
		virtual void Release() = 0;
		virtual const bool GetActvate() = 0;
				
		//Components Functions;
		void AddComponent(Component::IComponent*);
		void RemoveComponent(const char*);
		void RemoveAllComponents();		
		Component::IComponent* GetComponent(char*);
		const bool IsContainComponent(char*);

		//Variables
	protected:		
		bool mbActivated;
	private:
		MAP_COMPONENT ContainComponents;
	};
}