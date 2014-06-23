#pragma once
#include "../ShadeEngine.h"
#include "IComponent.h"


namespace Component {

	class Render : public IComponent
	{
	public:
		Render();
		~Render();
		void ResetComponent();
		void Start();
		void Update(float);
		void End();
		void DrawOnScreen(const LPD3DXEFFECT);

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

}