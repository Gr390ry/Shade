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
		Component::Transform* transform = GetComponent<Component::Transform>();

		if (transform)
		{
			Vector3 vRotate = transform->GetRollPitchYaw();
			vRotate.y += 2 * pDelta;
			transform->SetRollPitchYaw(vRotate);
		}

		ITER_COMPONENT iter = ContainComponents.begin();
		int nSize = ContainComponents.size();
		concurrency::parallel_for(0, nSize, [&](int n)
		{
			iter->second->Update(pDelta);
			++iter;
		});
	}

	void Actor::Release()
	{
		RemoveAllComponents();
	}

	const bool Actor::GetActvate() { return mbActivated; }
}