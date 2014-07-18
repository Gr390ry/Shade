#pragma once
#include "ShadeEngine.h"
#include "ISingleton.h"
#include "../Management/RenderDevice.h"

/*
�� ����Ʈ�� ����Ʈ ���Ϻ��� �ϳ��� ����Ʈ ��ü�� �����ؾ� �Ѵ�.
�ƿ� �ν��Ͻ̶����� ������ �ٽ� ������ ����� �ϴٴ� -_-...
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

		//effect ���� ������
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
		�������� �Ϸ�� ���̴��� ����Ʈ ���Ϸ� �����.
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