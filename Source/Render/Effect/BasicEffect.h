#pragma once
#include <ShadeEngine.h>
#include "../IEffect.h"

namespace Render { namespace Effect {

	class BasicEffect : public IEffect, public ISingleton<BasicEffect>
	{
	private:
		void SetInputLayout() override;

	public:
		void Initialize() override;
		void Release() override;


		void SetWorldMatrix(const float*);
		void SetViewProjectionMatrix(const float*);
		void SetLightDirection(const float*);

		ID3D11InputLayout* GetLayout() { return _inputLayout; }
		ID3DX11EffectTechnique* GetTechique() { return _technique; }
	private:
		ID3D11InputLayout*				_inputLayout;
		ID3DX11EffectTechnique*			_technique;
		ID3DX11EffectMatrixVariable*	_world;
		ID3DX11EffectMatrixVariable*	_viewProjection;
		ID3DX11EffectVectorVariable*	_lightDirection;
	};

}}