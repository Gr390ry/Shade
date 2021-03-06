#pragma once
#include "../Component/IComponent.h"
#include <map>




namespace Component {
	class IComponent;
};

namespace GameObject {

#define UPDATE_COMPONENTS(deltaTime)\
	ITER_COMPONENT iter = ContainComponents.begin();\
	int size = this->ContainComponents.size();\
	concurrency::parallel_for(0, size, [&](int n) { iter->second->Update(deltaTime); ++iter; });\

	class IGameObject
	{
	protected:
		//type defines
		typedef std::map<unsigned int, Component::IComponent*> MAP_COMPONENT;
		typedef std::pair<unsigned int, Component::IComponent*> PAIR_COMPONENT;
		typedef MAP_COMPONENT::iterator ITER_COMPONENT;

		//Functions
	public:
		virtual void Initialize() = 0;
		virtual void Release() = 0;
		virtual void Update(float) = 0;
		
		virtual const bool GetActvate() = 0;
				
		//Components Functions;
		template <typename T>
		T* AddComponent()
		{
			unsigned int	nTypeID = RTTI::GetTypeID<T>();
			ITER_COMPONENT	iter	= ContainComponents.find(nTypeID);
			if (iter != ContainComponents.end())
			{
				return static_cast<T*>((*iter).second);
			}

			Component::IComponent* component = new T;
			component->SetOwner(this);
			component->Initialize();

			PAIR_COMPONENT pair(nTypeID, component);
			ContainComponents.insert(pair);

			return static_cast<T*>(component);
		}
		template <typename T>
		T* GetComponent()
		{
			if (ContainComponents.size() == 0) return nullptr;

			unsigned int nTypeID = RTTI::GetTypeID<T>();
			Component::IComponent* component = ContainComponents.at(nTypeID);

			return static_cast<T*>(component);
		}
		template <typename T>
		void RemoveComponent()
		{
			unsigned int nTypeID = RTTI::GetTypeID<T>();

			ITER_COMPONENT iter = ContainComponents.find(nTypeID);

			if (iter != ContainComponents.end())
			{
				ContainComponents.erase(iter); //리스트에서 제거

				Component::IComponent* component = (*iter).second;
				
				component->Release();

				delete component;
				component = nullptr;
				//ConstructHelper::RemoveComponent(component); //메모리 해제
			}
		}
		void RemoveAllComponents()
		{
			ITER_COMPONENT iter = ContainComponents.begin();

			while (iter != ContainComponents.end())
			{
				ContainComponents.erase(iter);

				Component::IComponent* component = (*iter).second;
				//ConstructHelper::RemoveComponent(component); //메모리 해제

				component->Release();

				delete component;
				component = nullptr;

				++iter;
			}
		}
		
		//Variables
	protected:		
		bool				_IsActivated;
		MAP_COMPONENT		ContainComponents;
	};
}