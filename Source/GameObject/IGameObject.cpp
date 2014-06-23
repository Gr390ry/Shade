#include "IGameObject.h"
//#include "../ConstructHelper.h"
//#include "../Component/IComponent.h"


namespace GameObject {

	void IGameObject::AddComponent(Component::IComponent* component)
	{
		if (component == nullptr) { return; }

		char* id = const_cast<char*>(component->GetID());

		if (!IsContainComponent(id))
		{
			component->SetOwner(this);

			PAIR_COMPONENT pair(id, component);
			ContainComponents.insert(pair);
		}
	}

	void IGameObject::RemoveComponent(const char* identity)
	{
		ITER_COMPONENT iter = ContainComponents.find(const_cast<char*>(identity));

		if (iter != ContainComponents.end())
		{
			ContainComponents.erase(iter); //리스트에서 제거

			Component::IComponent* component = (*iter).second;
			ConstructHelper::RemoveComponent(component); //메모리 해제
		}
	}

	void IGameObject::RemoveAllComponents()
	{
		ITER_COMPONENT iter = ContainComponents.begin();

		while (iter != ContainComponents.end())
		{
			ContainComponents.erase(iter);

			Component::IComponent* component = (*iter).second;
			ConstructHelper::RemoveComponent(component); //메모리 해제

			++iter;
		}
	}

	//template <typename T>
	Component::IComponent* IGameObject::GetComponent(char* identity)
	{
		if (ContainComponents.size() == 0 ||
			!IsContainComponent(identity)) return nullptr;

		Component::IComponent* component = ContainComponents.at(identity);
		return component;
	}

	const bool IGameObject::IsContainComponent(char* identity)
	{
		if (ContainComponents.size() == 0) return false;
		ITER_COMPONENT iter = ContainComponents.find(identity);
		return (iter != ContainComponents.end());
	}

}; /*GameObject*/