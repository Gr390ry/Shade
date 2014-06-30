#pragma once
#include "../ShadeEngine.h"
#include "IRenderable.h"


namespace Component {

	
	class Transform;
	class Render : public IRenderable
	{
		DeclareRTTI(Render);

	public:
		
		struct Vertex
		{
			XMFLOAT3 position;
			XMFLOAT3 normal;
			XMFLOAT2 uv;
		};

	private:
		void CreateVertexBuffer();
		void CreateIndexBuffer();

	public:
		Render();
		~Render();
		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);

		void LoadFbxModel(const char*);
		
		//void DrawOnScreen(const LPD3DXEFFECT);
		void DrawFbxMesh();

		const char* GetID() { return "Render"; }
		const EFamily::_ GetFamilyID() { return EFamily::Render; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }
		//void SetMesh(LPD3DXMESH mesh) { pMesh = mesh; }

	private:
		//LPD3DXEFFECT		pShader;
		//LPD3DXMESH			pMesh;
		//LPDIRECT3DTEXTURE9	pDiffuseMap;
		//LPDIRECT3DTEXTURE9	pNormalMap;
		//LPDIRECT3DTEXTURE9	pSpecularMap;
		ID3D11Buffer*		pVB;
		ID3D11Buffer*		pIB;
		ID3D10EffectMatrixVariable* fxWorldVoewProjectionMatrix;
		//ID3D11InputLayout*	pInputLayout;

		std::vector<Vertex> mVertices;
		std::vector<int>	mIndices;

		XMMATRIX*			pWorldMatrix;
		bool				bIsCulled;
		Transform*			transform;
	};
	
};