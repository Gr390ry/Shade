#pragma once
#include "../IEffect.h"

namespace Render { namespace Effect {

	class InstancedBasic : IEffect
	{
	public:
		void Initialize() override;
		void Release() override;
		

		void SetWorldMatrix(const XMMATRIX*);
		void SetViewProjectionMatrix(const XMMATRIX*);
		void SetLightDirection(const XMVECTOR*);

	private:
		ID3DX11EffectMatrixVariable*	_world;
		ID3DX11EffectMatrixVariable*	_viewProjection;
		ID3DX11EffectVectorVariable*	_lightDirection;
	};

} /*Render*/ } /*Effect*/