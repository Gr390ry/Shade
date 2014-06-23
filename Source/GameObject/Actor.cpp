#include "Actor.h"
//#include "../ShadeEngine.h"
//#include "../Component/IComponent.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"

namespace GameObject {

	Actor::Actor() : pRender(nullptr), pTransform(nullptr)
	{
	}

	Actor::~Actor()
	{
		Release();
	}

	void Actor::Initialize()
	{
		if (pRender == nullptr) 
			pRender = ConstructHelper::CreateComponent<Component::Render>();
		if (pTransform == nullptr)
			pTransform = ConstructHelper::CreateComponent<Component::Transform>();

		AddComponent(pRender);
		AddComponent(pTransform);

		Component::Render* render = static_cast<Component::Render*>(GetComponent("Render"));
		render->SetMesh(ConstructHelper::LoadXMesh("Contents/Model/Torus.x"));
		render->SetWorldMatrix(&pTransform->GetWorldMatrix());
	}

	void Actor::Update(float pDelta)
	{
		pTransform->Update(pDelta);
		pRender->Update(pDelta);
	}

	void Actor::Release()
	{
		ConstructHelper::RemoveComponent(pRender);
		ConstructHelper::RemoveComponent(pTransform);
	}

	const bool Actor::GetActvate() { return mbActivated; }
	Component::Transform* Actor::GetTransform() { return pTransform; }
	const Component::Render* Actor::GetRender() { return pRender; }
}