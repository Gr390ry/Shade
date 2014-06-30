#include "ConstructHelper.h"
#include "Component/IComponent.h"
#include "Management/RenderDevice.h"
#include <type_traits>

//LPD3DXMESH ConstructHelper::LoadXMesh(const char* filepath)
//{
//	LPD3DXMESH ret = nullptr;
//
//	if (FAILED(D3DXLoadMeshFromX(filepath, D3DXMESH_SYSTEMMEM, RenderDevice::Get()->GetDevice(), nullptr, nullptr, nullptr, nullptr, &ret)))
//	{
//		OutputDebugString("Model Load failure:");
//		OutputDebugString(filepath);
//		OutputDebugString("\n");
//	}
//	return ret;
//}
//
//LPD3DXEFFECT ConstructHelper::LoadShader(const char* filepath)
//{
//	LPD3DXEFFECT ret = nullptr;
//	LPD3DXBUFFER error = nullptr;
//	DWORD shaderFlag = 0;
//
//#if _DEBUG
//	shaderFlag |= D3DXSHADER_DEBUG;
//#endif
//
//	D3DXCreateEffectFromFile(RenderDevice::Get()->GetDevice(), filepath, nullptr, nullptr,
//		shaderFlag, nullptr, &ret, &error);
//
//	if (!ret && error)
//	{
//		assert(0);
//
//		int size = error->GetBufferSize();
//		void* ack = error->GetBufferPointer();
//
//		if (ack)
//		{
//			char* str = new char[size];
//			const char* format = (const char*)ack;
//			sprintf_s(str, sizeof(str), format, size);
//			OutputDebugString(str);
//			delete[] str;
//		}
//	}
//
//	return ret;
//}
//LPDIRECT3DTEXTURE9 ConstructHelper::LoadTexture(const char* filepath)
//{
//	LPDIRECT3DTEXTURE9 ret = nullptr;
//
//	if (FAILED(D3DXCreateTextureFromFile(RenderDevice::Get()->GetDevice(), filepath, &ret)))
//	{
//		OutputDebugString("Texture Load failure:");
//		OutputDebugString(filepath);
//		OutputDebugString("\n");
//	}
//	return ret;
//}

template <class T>
T* ConstructHelper::CreateComponent()
{
	T* p = new T;
	Component::IComponent* component = dynamic_cast<Component::IComponent*>(p);

	if (component)
		component->Initialize();
	else
		delete p;

	return p;
}
void ConstructHelper::RemoveComponent(Component::IComponent* pComponent)
{
	if (pComponent)
	{
		pComponent->Release();
		delete pComponent;
		pComponent = nullptr;
	}
}