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
		void				Draw();
		void				SetMeshData(IMesh*);
	private:
		ID3D11Buffer*		pVB;
		ID3D11Buffer*		pIB;
		ID3DX11EffectTechnique* _tech;
		ID3D11InputLayout*				_inputLayout;
		//ID3D10EffectMatrixVariable* fxWorldVoewProjectionMatrix;
		//ID3D11ShaderResourceView*	pDiffuseMap;
		ID3D11Texture2D*			pDiffuseTexture;
		

		XMMATRIX*			_worldMatrix;
		bool				_IsCulled;
		Transform*			_transform;
		LPMESH				_meshData;
	};
	
};