#pragma once
#include "ISingleton.h"
#include <vector>

namespace Component {
	class Render;
}

class RenderDevice : public ISingleton<RenderDevice>
{
private:
	void RenderTarget(const LPD3DXEFFECT&, const LPD3DXMESH&, const DWORD&, const DWORD&);
	bool InitializeBuffer();
	bool InitializeFullScreenQuad();
	void DrawStencilBuffer();

public:	
	bool InitializeDevice(HWND);
	void Release();

	void RenderFrame();

	void AddListener(Component::Render*);
	void RemoveListener(Component::Render*);

	//TODO:Shader DEMO
	void LoadAsset();
	void Draw(const Matrix4x4&, const Matrix4x4&, const Matrix4x4&);

	const LPDIRECT3DDEVICE9 GetDevice();
private:
	LPDIRECT3D9 directObject;
	LPDIRECT3DDEVICE9 directDevice;

	LPD3DXEFFECT mpCreateShadowShader;
	LPD3DXEFFECT mpShader;


	LPDIRECT3DTEXTURE9 mpDiffuseTexture;
	LPDIRECT3DTEXTURE9 mpSpecularTexture;
	LPDIRECT3DTEXTURE9 mpNormalTexture;
	LPD3DXMESH mpModel;
	LPD3DXMESH mpDisc;

	std::vector<Component::Render*> listRenders;

	LPDIRECT3DTEXTURE9 mpShadowRenderTarget;
	LPDIRECT3DSURFACE9 mpShadowDepthStencil;

	LPDIRECT3DVERTEXDECLARATION9	mpFullScreenQuadDecl;
	LPDIRECT3DVERTEXBUFFER9			mpFullScreenQuadVB;
	LPDIRECT3DINDEXBUFFER9			mpFullScreenQuadIB;

	LPD3DXEFFECT mpNoEffect;
	LPD3DXEFFECT mpGrayScale;
	LPD3DXEFFECT mpSepia;
	LPDIRECT3DTEXTURE9 mpScreenRenderTarget;

	//for deferred
	LPDIRECT3DTEXTURE9 mpMultiRenderTarget[4];
};