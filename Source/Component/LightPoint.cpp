#include "LightPoint.h"
#include "../GameObject/IGameObject.h"

namespace Component {

	ImplementRTTI(LightPoint, IRenderable);

	LightPoint::LightPoint() { }
	LightPoint::~LightPoint() { }
	void LightPoint::ResetComponent() { }
	void LightPoint::Initialize() { }
	void LightPoint::Release() { }
	void LightPoint::Update(float) { }

}; /*Component*/