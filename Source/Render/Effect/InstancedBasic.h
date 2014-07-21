#pragma once
#include "../IEffect.h"
#include "ISingleton.h"

namespace Render { namespace Effect {

	class InstancedBasic : public IEffect, public ISingleton<InstancedBasic>
	{
	private:
		void SetInputLayout() override;

	public:
		void Initialize() override;
		void Release() override;
		

		void SetWorldMatrix(const XMMATRIX*);
		void SetViewProjectionMatrix(const XMMATRIX*);
		void SetLightDirection(const XMVECTOR*);

		ID3D11InputLayout* GetLayout() { return _inputLayout; }
		ID3DX11EffectTechnique* GetTechique() { return _technique; }
	private:
		ID3D11InputLayout*				_inputLayout;
		ID3DX11EffectTechnique*			_technique;
		ID3DX11EffectMatrixVariable*	_world;
		ID3DX11EffectMatrixVariable*	_viewProjection;
		ID3DX11EffectVectorVariable*	_lightDirection;
	};

} /*Render*/ } /*Effect*/