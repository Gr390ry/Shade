#include "General.h"
#include "GameObject\Actor.h"
#include "Component\Transform.h"

using namespace GameObject;

General::General() : pTestActor(nullptr)
{
}

bool General::InitializeGame()
{
	if (pTestActor == nullptr)
		pTestActor = new GameObject::Actor;

	pTestActor->Initialize();
	pTestActor->GetComponent<Component::Transform>()->SetScale(Vector3(0.5, 0.5, 0.5));
	pTestActor->GetComponent<Component::Transform>()->SetPosition(Vector3(-100, 50, 100));
	/*pTestActor->GetTransform()->SetScale(Vector3(1, 1, 1));
	pTestActor->GetTransform()->SetPosition(Vector3(-100, 0, 100));*/
	return true;
}

void General::Release()
{
}

void General::Update(float pDelta)
{
	pTestActor->Update(pDelta);
}