/*

	에셋 로드 관련 전역 함수 처리

*/
#include "Component\IComponent.h"
#include "RenderDevice.h"
#include <type_traits>


namespace ConstructHelper {

	static LPD3DXMESH LoadXMesh(const char* filepath)
	{
		LPD3DXMESH ret = nullptr;

		if (FAILED(D3DXLoadMeshFromX(filepath, D3DXMESH_SYSTEMMEM, RenderDevice::Get()->GetDevice(), nullptr, nullptr, nullptr, nullptr, &ret)))
		{
			OutputDebugString("Model Load failure:");
			OutputDebugString(filepath);
			OutputDebugString("\n");
		}
		return ret;
	}

	static LPD3DXEFFECT LoadShader(const char* filepath)
	{
		LPD3DXEFFECT ret = nullptr;
		LPD3DXBUFFER error = nullptr;
		DWORD shaderFlag = 0;

#if _DEBUG
		shaderFlag |= D3DXSHADER_DEBUG;
#endif

		D3DXCreateEffectFromFile(RenderDevice::Get()->GetDevice(), filepath, nullptr, nullptr,
			shaderFlag, nullptr, &ret, &error);

		if (!ret && error)
		{
			assert(0);

			int size = error->GetBufferSize();
			void* ack = error->GetBufferPointer();

			if (ack)
			{
				char* str = new char[size];
				const char* format = (const char*)ack;
				sprintf_s(str, sizeof(str), format, size);
				OutputDebugString(str);
				delete[] str;
			}
		}

		return ret;
	}
	static LPDIRECT3DTEXTURE9 LoadTexture(const char* filepath)
	{
		LPDIRECT3DTEXTURE9 ret = nullptr;

		if (FAILED(D3DXCreateTextureFromFile(RenderDevice::Get()->GetDevice(), filepath, &ret)))
		{
			OutputDebugString("Texture Load failure:");
			OutputDebugString(filepath);
			OutputDebugString("\n");
		}
		return ret;
	}

	template <class T>
	static T* CreateComponent()
	{
		T* p = new T;
		Component::IComponent* component = dynamic_cast<Component::IComponent*>(p);

		if (component)
			component->Start();
		else
			delete p;

		return p;
	}
	static void RemoveComponent(Component::IComponent* pComponent)
	{
		if (pComponent)
		{
			pComponent->End();
			delete pComponent;
			pComponent = nullptr;
		}
	}
};