#pragma once
#include "../ShadeEngine.h"
#include "IRenderable.h"


namespace Component {

	class Render : public IRenderable
	{
		DeclareRTTI(Render);
	public:
		Render();
		~Render();
		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);
		
		void DrawOnScreen(const LPD3DXEFFECT);

		const char* GetID() { return "Render"; }
		const EFamily::_ GetFamilyID() { return EFamily::Render; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }

		void SetMesh(LPD3DXMESH);
		void SetWorldMatrix(const Matrix4x4*);

	private:
		LPD3DXEFFECT		pShader;
		LPD3DXMESH			pMesh;
		LPDIRECT3DTEXTURE9	pDiffuseMap;
		LPDIRECT3DTEXTURE9	pNormalMap;
		LPDIRECT3DTEXTURE9	pSpecularMap;
		Matrix4x4*			pWorldMatrix;
		bool				bIsCulled;
	};
	
};