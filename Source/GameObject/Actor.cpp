#include "Actor.h"
#include "ConstructHelper.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"
#include "../Render/Mesh/BoxMesh.h"
#include "../Render/Mesh/SphereMesh.h"

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
	}

	void Actor::Update(float deltaTime)
	{
		UPDATE_COMPONENTS(deltaTime);
	}

	void Actor::Release()
	{
		RemoveAllComponents();
	}

	const bool Actor::GetActvate()
	{
		return _IsActivated;
	}
	const Component::Transform* Actor::GetTransform()
	{
		return GetComponent<Component::Transform>();
	}
	const Component::Render* Actor::GetRender()
	{
		return GetComponent<Component::Render>();
	}
}