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


	//bool InitializeDevice(HWND);
	bool InitializeDevice11(HWND);
	void Release();

	void OnResize();
	void Render11();

	void AddListener(Component::Render*);
	void RemoveListener(Component::Render*);

	//TODO:Shader DEMO
	void LoadAsset();

	FbxManager* GetFbxManager();
	ID3D11Device* GetDevice() { return directDevice11; }
	ID3D11DeviceContext* GetContext() { return directContext; }

private:
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

	ID3D11InputLayout*		inputLayout;

	std::vector<Component::Render*> listRenders;
	std::vector<Component::LightPoint*> listLightPoints;

	//��� ������ ������Ʈ���� �޴´�.(Render, LightPoint, Etc..)
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