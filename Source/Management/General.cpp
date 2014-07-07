#include "General.h"
#include "../GameObject/Actor.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"
#include "../Component/CameraProp.h"
#include "../GameObject/Camera.h"
#include "../Mesh/IMesh.h"
#include "../Mesh/BoxMesh.h"
#include "../Mesh/SphereMesh.h"

using namespace GameObject;

General::General() : pTestActor(nullptr), pMainCamera(nullptr)
{
}

bool General::InitializeGame()
{
	GameObject::Actor* pActor = nullptr;
	IMesh* pBoxMesh = new BoxMesh;
	pBoxMesh->Initialize("");

	//fillow
	for (int i = 0; i < 30; ++i)
	{
		pActor = new GameObject::Actor;
		pActor->Initialize();

		Component::Render* pRender = pActor->GetComponent<Component::Render>();
		Component::Transform* pTransform = pActor->GetComponent<Component::Transform>();

		if (pRender)
		{
			pRender->SetMeshData(pBoxMesh);
		}
		if (pTransform)
		{
			pTransform->SetScale(XMFLOAT3(15, 50, 15));
			pTransform->SetPosition(XMFLOAT3((float)((i % 2) * 300 - 150), -25, (float)((i / 2) * 200 - 100)));
		}
		listActors.emplace_back(pActor);
		pActor = nullptr;
	}
	//floor
	pActor = new GameObject::Actor;
	pActor->Initialize();
	Component::Render* pRender = pActor->GetComponent<Component::Render>();
	Component::Transform* pTransform = pActor->GetComponent<Component::Transform>();
	if (pRender)
	{
		pRender->SetMeshData(pBoxMesh);
	}
	if (pTransform)
	{
		pTransform->SetScale(XMFLOAT3(300, 1, 2000));
		pTransform->SetPosition(XMFLOAT3(0, -100, 2000));
	}
	listActors.emplace_back(pActor);

	if (pMainCamera == nullptr)
		pMainCamera = new GameObject::Camera;

	pMainCamera->Initialize();
	pMainCamera->SetPosition(XMFLOAT3(GENERIC::worldCameraPosition.x, GENERIC::worldCameraPosition.y, GENERIC::worldCameraPosition.z));
	pMainCamera->SetLookAt(XMFLOAT3(0, 0, 0));

	return true;
}

void General::Release()
{
	for (GameObject::Actor* pActor : listActors)
	{
		pActor->Release();
		SAFE_DELETE(pActor);
	}
	//pTestActor->Release();
	pMainCamera->Release();

	//SAFE_DELETE(pTestActor);
	SAFE_DELETE(pMainCamera);
}

void General::Update(float pDelta)
{
	for (GameObject::Actor* pActor : listActors)
	{
		if (pActor == nullptr) continue;
		pActor->Update(pDelta);
	}
	UPDATE_OBJECT(pMainCamera, pDelta);
}

Camera* General::GetMainCamera() const
{
	return pMainCamera;
}