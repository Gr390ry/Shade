#pragma once
#include "IComponent.h"

namespace Component {

	//컴포넌트 분류를 위한 인터페이스 클래스
	class IRenderable : public IComponent
	{
		DeclareRTTI(IRenderable);
	};

}; /*Component*/