#pragma once
#include "../ISingleton.h"

namespace GameObject {
	class Actor;
	class Camera;
}

class General : public ISingleton<General>
{
public:	
	General();

	bool InitializeGame();
	void Release();
	void Update(float);

	GameObject::Camera* GetMainCamera() const;

private:
	GameObject::Actor* pTestActor;
	GameObject::Camera* pMainCamera;
};