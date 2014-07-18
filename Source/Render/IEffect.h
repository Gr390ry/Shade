#pragma once
#include "ShadeEngine.h"
#include "ISingleton.h"
#include "../Management/RenderDevice.h"

/*
각 이펙트는 이펙트 파일별로 하나의 이펙트 객체만 생성해야 한다.
아오 인스턴싱때문에 구조를 다시 뒤집어 엎어야 하다니 -_-...
*/

namespace Render { namespace Effect {

	class IEffect
	{
	public:
		IEffect();
		IEffect(IEffect& ref) = delete;
		virtual ~IEffect();

		
		virtual void Initialize() = 0;
		virtual void Release() = 0;

		bool LoadEffect(const std::string&);
		const LPD3D11EFFECT GetFx();
	protected:
		LPD3D11EFFECT	object;
	};

	IEffect::IEffect() : object(nullptr)
	{
	}
	IEffect::~IEffect()
	{
		SAFE_RELEASE(object);
	}

	bool IEffect::LoadEffect(const std::string& filepath)
	{
		DWORD shaderFlag = 0;

#if defined(DEBUG) || defined(_DEBUG)
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* compilationMsgs = nullptr;

		HRESULT hr;
		ID3D11Device* device = RenderDevice::Get()->GetDevice();

		hr = D3DX11CompileFromFile(filepath.c_str(), 0, 0, 0, "fx_5_0",
									shaderFlag, 0, 0,
									&compiledShader, &compilationMsgs, 0);

		//effect 파일 컴파일
		if (FAILED(hr))
		{
			if (compilationMsgs != 0)
			{
				MessageBoxA(NULL, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
				SAFE_RELEASE(compilationMsgs);
			}
			assert(NULL);
			return false;
		}

		/* - NOTICE -
		컴파일이 완료된 쉐이더를 이펙트 파일로 만든다.
		*/
		

		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			0, device, &object);
		SAFE_RELEASE(compiledShader);

		if (FAILED(hr)) return false;
		return true;

		Initialize();
	}

	const LPD3D11EFFECT IEffect::GetFx()
	{
		return object;
	}

} /*Effect*/ } /*Render*/