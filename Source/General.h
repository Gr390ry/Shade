#pragma once
#include "ISingleton.h"

namespace GameObject {
	class Actor;
}

class General : public ISingleton<General>
{
public:	
	bool InitializeGame();
	void Release();
	void Update(float);

private:
	GameObject::Actor* pTestActor;
};