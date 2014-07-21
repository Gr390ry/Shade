#pragma once
#include "ISingleton.h"
#include "ShadeEngine.h"

namespace GameObject {
	class Actor;
	class Camera;
}

class General : public ISingleton<General>
{
	typedef std::vector<GameObject::Actor*> VEC_ACTOR;
	typedef std::vector<XMMATRIX> VEC_MATRIX;

public:	
	General();

	bool InitializeGame();
	bool LoadAssets();
	void Release();
	void Update(float);

	GameObject::Camera* GetMainCamera() const;
	VEC_ACTOR GetActorList();
	VEC_MATRIX GetInstancedObjectsWorldMatrix();

private:
	VEC_ACTOR				_vecActors;
	VEC_MATRIX				_vecInstanObjectWorld;
	GameObject::Camera*		_mainCamera;


};