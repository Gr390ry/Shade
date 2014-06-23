#include "Actor.h"
//#include "../ShadeEngine.h"
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

		pTransform->Update(0);

		pRender->SetMesh(ConstructHelper::LoadXMesh("Contents/Model/Torus.x"));
		pRender->SetWorldMatrix(&pTransform->GetWorldMatrix());
		//pTransform->GetPosition();
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

	void Actor::AddComponent(Component::IComponent* component)
	{
	}

	const bool Actor::GetActvate() { return mbActivated; }
	Component::Transform* Actor::GetTransform() { return pTransform; }
	const Component::Render* Actor::GetRender() { return pRender; }
}