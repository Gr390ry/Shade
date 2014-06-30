#include "General.h"
#include "../GameObject/Actor.h"
#include "../Component/Transform.h"
#include "../Component/CameraProp.h"
#include "../GameObject/Camera.h"

using namespace GameObject;

General::General() : pTestActor(nullptr), pMainCamera(nullptr)
{
}

bool General::InitializeGame()
{
	if (pTestActor == nullptr)
		pTestActor = new GameObject::Actor;
	if (pMainCamera == nullptr)
		pMainCamera = new GameObject::Camera;

	/*pTestActor->Initialize();
	pTestActor->GetComponent<Component::Transform>()->SetScale(Vector3(0.5, 0.5, 0.5));
	pTestActor->GetComponent<Component::Transform>()->SetPosition(Vector3(-100, 50, 100));*/

	pMainCamera->Initialize();
	pMainCamera->SetPosition(XMFLOAT3(GENERIC::worldCameraPosition.x, GENERIC::worldCameraPosition.y, GENERIC::worldCameraPosition.z));
	pMainCamera->SetLookAt(XMFLOAT3(0, 0, 0));

	return true;
}

void General::Release()
{
	pTestActor->Release();
	pMainCamera->Release();

	SAFE_DELETE(pTestActor);
	SAFE_DELETE(pMainCamera);
}

void General::Update(float pDelta)
{
	UPDATE_OBJECT(pTestActor, pDelta);
	UPDATE_OBJECT(pMainCamera, pDelta);
}

Camera* General::GetMainCamera() const
{
	return pMainCamera;
}