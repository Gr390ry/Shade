#pragma once
#include "ISingleton.h"
#include "ShadeEngine.h"
#include <vector>
#include <fbxsdk.h>

//using namespace fbxsdk_2015_1;

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

	bool CreateEffectFile(char*, LPD3D11EFFECT*);
	bool InitializeVertexLayout();

public:	
	RenderDevice();


	//bool InitializeDevice(HWND);
	bool InitializeDevice11(HWND);
	void Release();

	void OnResize();
	void Render11();

	void AddListener(Component::Render*);
	void RemoveListener(Component::Render*);

	void LoadAsset();

	FbxManager* GetFbxManager() { return _fbxManager; }
	ID3D11Device* GetDevice() { return _directDevice11; }
	ID3D11DeviceContext* GetContext() { return _directContext; }

private:
	

	ID3D11Device*			_directDevice11;
	ID3D11DeviceContext*	_directContext;
	D3D_FEATURE_LEVEL		_featureLevel;
	IDXGISwapChain*			_swapChain;
	ID3D11Texture2D*		_depthStencilBuffer;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT			_screenViewPort;
	D3D_DRIVER_TYPE			_driverType;
	bool					_enableMSAAx4;
	ID3D11Buffer*			_instanceBuffer;
	
	//LPD3D11EFFECT			mShader;
	//ID3D11InputLayout*		mInputLayout;
	//ID3DX11EffectTechnique* mTechnique;

	std::vector<Component::Render*> _listRenders;
	std::vector<Component::LightPoint*> _listLightPoints;

	//출력 가능한 컴포넌트들을 받는다.(Render, LightPoint, Etc..)
	MAP_COMPONENT			_containComponents;
	FbxManager*				_fbxManager;		
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