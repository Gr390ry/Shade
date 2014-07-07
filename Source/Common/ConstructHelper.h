/*

	���� �ε� ���� ���� �Լ� ó��

*/
#pragma once
#include "ShadeEngine.h"

namespace Component {
	class IComponent;
}; /*Component*/
namespace GameObject{
	class IGameObject;
};

class ConstructHelper
{
public:

	virtual ~ConstructHelper() {}

	//static LPD3DXMESH LoadXMesh(const char*);
	//static LPD3DXEFFECT LoadShader(const char*);
	//static LPDIRECT3DTEXTURE9 LoadTexture(const char*);

	template <class T>
	static T* CreateComponent();
	static void RemoveComponent(Component::IComponent*);
};