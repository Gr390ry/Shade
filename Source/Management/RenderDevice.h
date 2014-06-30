#pragma once
#include "../ISingleton.h"
#include "../ShadeEngine.h"
#include <vector>
#include <fbxsdk.h>

using namespace fbxsdk_2015_1;

namespace Component {
	class Render;
	class LightPoint;
	class IRenderable;
}

class RenderDevice : public ISingleton<RenderDevice>
{
	//typedef
	typedef std::map<unsigned int, Component::IRenderable*> MAP_COMPONENT;
	typedef MAP_COMPONENT::iterator ITER_COMPONENT;

private:
	//void RenderTarget(const LPD3DXEFFECT&, const LPD3DXMESH&, const DWORD&, const DWORD&);
	bool InitializeBuffer();
	bool InitializeFullScreenQuad();
	bool InitializeFbx();
	void DrawStencilBuffer();

public:	
	RenderDevice();


	bool InitializeDevice(HWND);
	bool InitializeDevice11(HWND);
	void Release();

	void OnResize();
	void RenderFrame();
	void Render11();

	void AddListener(Component::Render*);
	void RemoveListener(Component::Render*);

	//template <typename T>
	//void AddListener(const T*);
	//template <typename T>
	//void RemoveListener(const T*);

	//TODO:Shader DEMO
	void LoadAsset();
	//void Draw(const Matrix4x4&, const Matrix4x4&, const Matrix4x4&);

	//const LPDIRECT3DDEVICE9 GetDevice();
	FbxManager* GetFbxManager();
private:
	//LPDIRECT3D9				directObject;
	//LPDIRECT3DDEVICE9		directDevice;
	FbxManager*				fbxManager;

	ID3D11Device*			directDevice11;
	ID3D11DeviceContext*	directContext;
	D3D_FEATURE_LEVEL		featureLevel;
	IDXGISwapChain*			swapChain;
	ID3D11Texture2D*		depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT			screenViewPort;
	D3D_DRIVER_TYPE			driverType;
	bool					enableMSAAx4;

	//LPD3DXEFFECT			mpCreateShadowShader;
	//LPD3DXEFFECT			mpShader;
	//LPDIRECT3DTEXTURE9		mpDiffuseTexture;
	//LPDIRECT3DTEXTURE9		mpSpecularTexture;
	//LPDIRECT3DTEXTURE9		mpNormalTexture;
	//LPD3DXMESH				mpModel;

	std::vector<Component::Render*> listRenders;
	std::vector<Component::LightPoint*> listLightPoints;

	//LPDIRECT3DTEXTURE9 mpShadowRenderTarget;
	//LPDIRECT3DSURFACE9 mpShadowDepthStencil;

	//LPDIRECT3DVERTEXDECLARATION9	mpFullScreenQuadDecl;
	//LPDIRECT3DVERTEXBUFFER9			mpFullScreenQuadVB;
	//LPDIRECT3DINDEXBUFFER9			mpFullScreenQuadIB;

	//LPD3DXEFFECT mpNoEffect;
	//LPD3DXEFFECT mpGrayScale;
	//LPD3DXEFFECT mpSepia;
	//LPDIRECT3DTEXTURE9 mpScreenRenderTarget;

	//출력 가능한 컴포넌트들을 받는다.(Render, LightPoint, Etc..)
	MAP_COMPONENT ContainComponents;
		
	/*
		Support Maximize RenderTarget Count To DirectX Version
		-DX9 : 4
		-DX11 : 8

		Deferred Render Target Index Information
		[0] = Depth/StencilField
		[1] = Light Acculmulation/Intensity
		[2] = NormalXY (NormalZ = sqrt(1 - NormalX^2 - NormalY^2);
		[3] = Diffuse Albedo RGB / Sun_Occulsion	
	*/
	//LPD3DXEFFECT mpMultiSamplingShader;
	//LPD3DXEFFECT mpDeferredShader;
	//LPDIRECT3DTEXTURE9 mpMultiRenderTarget[4];
};

//
//template <typename T>
//void RenderDevice::AddListener(const T* component)
//{
//}
//template <typename T>
//void RenderDevice::RemoveListener(const T* component)
//{
//}