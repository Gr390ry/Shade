#pragma once
#include "../ShadeEngine.h"
#include "IRenderable.h"


namespace Component {

	
	class Transform;
	class Render : public IRenderable
	{
		DeclareRTTI(Render);

	public:
		class Attribute
		{
		public:
			int indices;
			Vector3 vertices;			
			Vector3 normal;
			Vector3 tangent;
			Vector3 binormal;
			Vector2 uv;

			Attribute();
		};

	public:
		Render();
		~Render();
		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);

		void LoadFbxModel(const char*);
		
		void DrawOnScreen(const LPD3DXEFFECT);
		void DrawFbxMesh();

		const char* GetID() { return "Render"; }
		const EFamily::_ GetFamilyID() { return EFamily::Render; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }
		void SetMesh(LPD3DXMESH mesh) { pMesh = mesh; }

	private:
		LPD3DXEFFECT		pShader;
		LPD3DXMESH			pMesh;
		LPDIRECT3DTEXTURE9	pDiffuseMap;
		LPDIRECT3DTEXTURE9	pNormalMap;
		LPDIRECT3DTEXTURE9	pSpecularMap;
		Matrix4x4*			pWorldMatrix;
		bool				bIsCulled;

		Attribute*			attributes;
		Transform*			transform;
	};
	
};