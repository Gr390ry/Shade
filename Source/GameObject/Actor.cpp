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
		render->LoadFbxModel("Contents/Model/darkSider2/DarSider.fbx");
	}

	void Actor::Update(float pDelta)
	{
		Component::Transform* transform = GetComponent<Component::Transform>();

		if (transform)
		{
			XMFLOAT3 vRotate = transform->GetRollPitchYaw();
			//vRotate.x = 0.785f;
			vRotate.y += 1 * pDelta;
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