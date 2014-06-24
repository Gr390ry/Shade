#include "Prop.h"
#include "../Component/Transform.h"

namespace GameObject {

	Prop::Prop()
	{
	}

	Prop::~Prop()
	{
	}

	void Prop::Initialize()
	{
		AddComponent<Component::Transform>();
	}

	void Prop::Release()
	{
		RemoveAllComponents();
	}

	void Prop::Update(float pDelta)
	{
	}

}; /*GameObject*/