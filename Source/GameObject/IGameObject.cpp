
#include "IGameObject.h"

namespace GameObject {

	//template <typename T>
	//void IGameObject::AddComponent()
	//{
	//	unsigned int nTypeID = RTTI::GetTypeID<T>();
	//	if (ContainComponents.find(nTypeID) == ContainComponents.end())
	//	{
	//		return;
	//	}

	//	Component::IComponent* component = new T;
	//	component->SetOwner(this);
	//	component->Start();

	//	PAIR_COMPONENT pair(nTypeID, component);
	//	ContainComponents.insert(pair);
	//}

	//template <typename T>
	//T* IGameObject::GetComponent()
	//{
	//	if (ContainComponents.size() == 0 ||
	//		!IsContainComponent(identity)) return nullptr;

	//	unsigned int nTypeID = RTTI::GetTypeID<T>();
	//	Component::IComponent* component = ContainComponents.at(nTypeID);

	//	return static_cast<T*>(component);
	//}

	//template <typename T>
	//void IGameObject::RemoveComponent()
	//{
	//	unsigned int nTypeID = RTTI::GetTypeID<T>();

	//	ITER_COMPONENT iter = ContainComponents.find(nTypeID);

	//	if (iter != ContainComponents.end())
	//	{
	//		ContainComponents.erase(iter); //리스트에서 제거

	//		Component::IComponent* component = (*iter).second;
	//		ConstructHelper::RemoveComponent(component); //메모리 해제
	//	}
	//}

	//void IGameObject::RemoveAllComponents()
	//{
	//	ITER_COMPONENT iter = ContainComponents.begin();

	//	while (iter != ContainComponents.end())
	//	{
	//		ContainComponents.erase(iter);

	//		Component::IComponent* component = (*iter).second;
	//		ConstructHelper::RemoveComponent(component); //메모리 해제

	//		++iter;
	//	}
	//}

	

	//template <typename T>
	//const bool IGameObject::IsContainComponent()
	//{
	//	if (ContainComponents.size() == 0) return false;

	//	unsigned int nTypeID = RTTI::GetTypeID<T>();
	//	ITER_COMPONENT iter = ContainComponents.find(identity);

	//	return (iter != ContainComponents.end());
	//}

}; /*GameObject*/