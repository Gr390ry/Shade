#include "Actor.h"
#include "../ConstructHelper.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"

namespace GameObject {

	Actor::Actor()
	{
	}

	Actor::~Actor()
	{
		Release();
	}

	void Actor::Initialize()
	{
		this->AddComponent<Component::Render>();
		this->AddComponent<Component::Transform>();
		
		Component::Transform* transform = GetComponent<Component::Transform>();
		Component::Render* render = GetComponent<Component::Render>();

		render->SetMesh(ConstructHelper::LoadXMesh("Contents/Model/Torus.x"));
		render->SetWorldMatrix(&transform->GetWorldMatrix());
	}

	void Actor::Update(float pDelta)
	{
		/*pTransform->Update(pDelta);
		pRender->Update(pDelta);*/
	}

	void Actor::Release()
	{
		/*ConstructHelper::RemoveComponent(pRender);
		ConstructHelper::RemoveComponent(pTransform);*/
	}

	const bool Actor::GetActvate() { return mbActivated; }
	/*Component::Transform* Actor::GetTransform() { return pTransform; }
	const Component::Render* Actor::GetRender() { return pRender; }*/
}