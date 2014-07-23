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
	IMesh*	meshData	= MeshPool::Get()->GetMeshData("Box");
	Actor*	actor		= new Actor();
	Component::Render* render = actor->GetComponent<Component::Render>();

	if (render)
	{
		render->SetMeshData(meshData);
	}
	_vecActors.emplace_back(actor);

	Console::Get()->print("Test Instancing World Matrix Complete[Count:%d]\n", _vecInstanObjectWorld.size());
	Console::Get()->print("Test Actor Complete[Count:%d]\n", _vecActors.size());

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
	typedef Render::Effect::BasicEffect basicEffect;

	InstancedBasic::Get()->LoadEffect("Contents/InstanceBasic.fx");
	basicEffect::Get()->LoadEffect("Contents/color.fx");
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