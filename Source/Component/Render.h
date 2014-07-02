#pragma once
#include "../ShadeEngine.h"
#include "IRenderable.h"


namespace Component {
	class Transform;
};
class IMesh;


namespace Component {		
	class Render : public IRenderable
	{
		DeclareRTTI(Render);

	//public:
	//	
	//	struct Vertex
	//	{
	//		XMFLOAT3 position;
	//		XMFLOAT3 normal;
	//		XMFLOAT2 uv;
	//	};

	private:
		void CreateVertexBuffer();
		void CreateIndexBuffer();

		//void GetFBXInfo(FbxNode*);

	public:
		Render();
		~Render();
		void ResetComponent();
		void Initialize();
		void Release();
		void Update(float);

		void LoadFbxModel(const char*);
		void DrawFbxMesh(ID3DX11EffectTechnique*);

		const char* GetID() { return "Render"; }
		const EFamily::_ GetFamilyID() { return EFamily::Render; }
		void SetOwner(GameObject::IGameObject* owner) { pOwner = owner; }
		

	private:
		ID3D11Buffer*		pVB;
		ID3D11Buffer*		pIB;
		ID3D10EffectMatrixVariable* fxWorldVoewProjectionMatrix;

		//std::vector<Vertex> mVertices;
		//std::vector<int>	mIndices;

		XMMATRIX*			pWorldMatrix;
		bool				bIsCulled;
		Transform*			transform;
		IMesh*				pMeshData;
	};
	
};