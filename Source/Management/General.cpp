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

	int batchCount = 10;
	float boxSize = 5;
	float boxPositionInterval = boxSize + 15;
	float boxLength = boxPositionInterval * batchCount;
	XMFLOAT3 center(0, 0, 0);

	for (int col = 0; col < batchCount; ++col)
	{
		for (int row = 0; row < batchCount; ++row)
		{
			for (int depth = 0; depth < batchCount; ++depth)
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
					pTransform->SetScale(XMFLOAT3(boxSize, boxSize, boxSize));					

					XMFLOAT3 position(boxPositionInterval * col + -boxLength * 0.5f + center.x,
						boxPositionInterval * row + -boxLength * 0.5f + center.y,
						boxPositionInterval * depth + -boxLength * 0.5f + center.z);

					Console::Get()->print("processing test box actor index[%d,%d,%d]\n", col, row, depth);


					pTransform->SetPosition(position);
				}
				listActors.emplace_back(pActor);
				pActor = nullptr;
			}
		}
	}

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

	pMainCamera->Release();
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