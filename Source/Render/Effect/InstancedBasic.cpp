#include "InstancedBasic.h"
#include "../Management/RenderDevice.h"

namespace Render { namespace Effect {

	void InstancedBasic::Initialize()
	{
		assert(object == nullptr);

		_world				= object->GetVariableByName("gWorld")->AsMatrix();
		_viewProjection		= object->GetVariableByName("gViewProjection")->AsMatrix();
		_lightDirection		= object->GetVariableByName("gLightDirection")->AsVector();
	}

	void InstancedBasic::Release()
	{
	}

	void InstancedBasic::SetWorldMatrix(const XMMATRIX* worldMatrix)
	{
		assert(worldMatrix == nullptr);
		_world->SetMatrix(reinterpret_cast<float*>(&worldMatrix));
	}

	void InstancedBasic::SetViewProjectionMatrix(const XMMATRIX* viewProjectionMatrix)
	{
		assert(viewProjectionMatrix == nullptr);
		_viewProjection->SetMatrix(reinterpret_cast<float*>(&viewProjectionMatrix));
	}

	void InstancedBasic::SetLightDirection(const XMVECTOR* lightDirection)
	{
		assert(lightDirection == nullptr);
		_lightDirection->SetFloatVector(reinterpret_cast<float*>(&lightDirection));
	}

} /*Effect*/ } /*Render*/