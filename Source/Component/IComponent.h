#pragma once

namespace GameObject {
	class IGameObject;
};

namespace Component {

	class EFamily
	{
	public:
		enum _
		{
			Render,
			System,
		};
	};
	
	class IComponent
	{
	public:
		virtual void ResetComponent() = 0;
		virtual void Start() = 0;
		virtual void Update(float) = 0;
		virtual void End() = 0;

		virtual const char* GetID() = 0;
		virtual const EFamily::_ GetFamilyID() = 0;
		virtual void SetOwner(GameObject::IGameObject*) = 0;

	protected:
		GameObject::IGameObject* pOwner;
	};
	
} /*Component*/