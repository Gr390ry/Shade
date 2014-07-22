#include "General.h"
#include "../GameObject/Actor.h"
#include "../Component/Render.h"
#include "../Component/Transform.h"
#include "../Component/CameraProp.h"
#include "../GameObject/Camera.h"
#include "../IncludeAssets.h"
#include "MeshPool.h"

using namespace GameObject;

General::General() : _mainCamera(nullptr)
{
}

bool General::InitializeGame()
{
	GameObject::Actor* pActor = nullptr;
	//IMesh* pBoxMesh = MeshPool::Get()->GetMeshData("Box");
	int batchCount = 2;
	float boxSize = 5;
	float boxPositionInterval = boxSize + 15;
	float boxLength = boxPositionInterval * batchCount;
	XMFLOAT3 center(0, 0, 0);

	for (int i = 0; i < 10; ++i)
	{
		XMMATRIX worldMatrix;
		XMFLOAT3 position = XMFLOAT3(i * 30, 0, -20);
		XMFLOAT3 scale = XMFLOAT3(boxSize, boxSize, boxSize);

		worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
		_vecInstanObjectWorld.emplace_back(worldMatrix);
	}

	//for (int col = 0; col < batchCount; ++col)
	//{
	//	for (int row = 0; row < batchCount; ++row)
	//	{
	//		for (int depth = 0; depth < batchCount; ++depth)
	//		{
	//			/*pActor = new GameObject::Actor;
	//			pActor->Initialize();

	//			Component::Render* render = pActor->GetComponent<Component::Render>();
	//			Component::Transform* transform = pActor->GetComponent<Component::Transform>();

	//			if (render)
	//			{
	//				render->SetMeshData(pBoxMesh);
	//			}
	//			if (transform)
	//			{
	//				transform->SetScale(XMFLOAT3(boxSize, boxSize, boxSize));

	//				XMFLOAT3 position(boxPositionInterval * col + -boxLength * 0.5f + center.x,
	//					boxPositionInterval * row + -boxLength * 0.5f + center.y,
	//					boxPositionInterval * depth + -boxLength * 0.5f + center.z);

	//				Console::Get()->print("processing test box actor index[%d,%d,%d]\n", col, row, depth);
	//				
	//				transform->SetPosition(position);
	//				transform->Update(0);

	//				_vecInstanObjectWorld.emplace_back(transform->GetWorldMatrix());
	//			}


	//			_vecActors.emplace_back(pActor);
	//			pActor = nullptr;*/

	//			XMMATRIX worldMatrix;
	//			XMFLOAT3 position = XMFLOAT3(boxPositionInterval * col + -boxLength * 0.5f + center.x,
	//										boxPositionInterval * row + -boxLength * 0.5f + center.y,
	//										boxPositionInterval * depth + -boxLength * 0.5f + center.z);
	//			XMFLOAT3 scale = XMFLOAT3(boxSize, boxSize, boxSize);

	//			worldMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	//			worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);
	//			_vecInstanObjectWorld.emplace_back(worldMatrix);
	//		}
	//	}
	//}

	Console::Get()->print("Test Instancing World Matrix Complete[Count:%d]\n", _vecInstanObjectWorld.size());

	if (_mainCamera == nullptr)
		_mainCamera = new GameObject::Camera;

	_mainCamera->Initialize();
	_mainCamera->SetPosition(XMFLOAT3(GENERIC::worldCameraPosition.x, GENERIC::worldCameraPosition.y, GENERIC::worldCameraPosition.z));
	_mainCamera->SetLookAt(XMFLOAT3(0, 0, 0));

	return true;
}

bool General::LoadAssets()
{
	typedef Render::Effect::InstancedBasic InstancedBasic;

	InstancedBasic::Get()->LoadEffect("Contents/InstanceBasic.fx");
	MeshPool::Get()->Initialize();

	return true;
}

void General::Release()
{
	for (GameObject::Actor* actor : _vecActors)
	{
		actor->Release();
		SAFE_DELETE(actor);
	}
	_vecActors.clear();
	_vecInstanObjectWorld.clear();

	_mainCamera->Release();
	SAFE_DELETE(_mainCamera);
}

void General::Update(float pDelta)
{
	for (GameObject::Actor* actor : _vecActors)
	{
		if (actor == nullptr) continue;
		actor->Update(pDelta);
	}
	UPDATE_OBJECT(_mainCamera, pDelta);
}

Camera* General::GetMainCamera() const
{
	return _mainCamera;
}

General::VEC_ACTOR General::GetActorList()
{
	return _vecActors;
}

General::VEC_MATRIX General::GetInstancedObjectsWorldMatrix()
{
	return _vecInstanObjectWorld;
}