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
		typedef IMesh* LPMESH;

		DeclareRTTI(Render);
	private:
		void CreateVertexBuffer();
		void CreateIndexBuffer();

	public:
		Render();
		~Render();
		void				ResetComponent()	override;
		void				Initialize()		override;
		void				Release()			override;
		void				Update(float)		override;
		const char*			GetID()				override { return "Render"; }
		const EFamily::_	GetFamilyID()		override { return EFamily::Render; }
		void				SetOwner(GameObject::IGameObject* owner) override { pOwner = owner; }

		//void				LoadFbxModel(const char*);
		void				RendMesh(ID3DX11EffectTechnique*);
		void				SetMeshData(IMesh*);
	private:
		ID3D11Buffer*		pVB;
		ID3D11Buffer*		pIB;
		ID3D10EffectMatrixVariable* fxWorldVoewProjectionMatrix;
		ID3D11Texture2D*	pDiffuseTexture;
		ID3D11ShaderResourceView* pDiffuseMap;

		XMMATRIX*			pWorldMatrix;
		bool				bIsCulled;
		Transform*			transform;
		LPMESH				meshData;
	};
	
};