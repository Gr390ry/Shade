#pragma once
#include "ISingleton.h"
#include "ShadeEngine.h"

namespace GameObject {
	class Actor;
	class Camera;
}

class General : public ISingleton<General>
{
public:	
	General();

	bool InitializeGame();
	bool LoadAssets();
	void Release();
	void Update(float);

	GameObject::Camera* GetMainCamera() const;

private:
	GameObject::Actor* pTestActor;
	std::vector<GameObject::Actor*> listActors;

	GameObject::Camera* pMainCamera;


};