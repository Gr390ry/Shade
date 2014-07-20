#include "RenderDevice.h"
#include "General.h"
#include "../Component/Render.h"
#include "../GameObject/Camera.h"
#include <algorithm>

RenderDevice::RenderDevice() : _fbxManager(nullptr),
							   _instanceBuffer(nullptr),
							   _directDevice11(nullptr),
							   _directContext(nullptr),
							   _swapChain(nullptr),
							   _depthStencilBuffer(nullptr),
							   _renderTargetView(nullptr)
{
}

void RenderDevice::Release()
{
	if (_fbxManager)
	{
		_fbxManager->Destroy();
	}

	SAFE_RELEASE(_instanceBuffer);
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_depthStencilBuffer);
	SAFE_RELEASE(_swapChain);
	SAFE_RELEASE(_directContext);
	SAFE_RELEASE(_directDevice11);
}

bool RenderDevice::InitializeDevice11(HWND hWnd)
{
	UINT uFlag = 0;

#ifdef _DEBUG
	uFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount			= 1;
	sd.BufferDesc.Width		= GENERIC::windowWidth;
	sd.BufferDesc.Height	= GENERIC::windowHeight;
	sd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow			= hWnd;
	sd.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags				= 0;
	sd.SampleDesc.Count		= 1;
	sd.SampleDesc.Quality	= 0;
	sd.Windowed				= TRUE;
	
	HRESULT hr = E_FAIL;
	//다이렉트 11 디바이스/컨텍스트/스왑체인 생성
	for (UINT i = 0; i < numDriverTypes; ++i)
	{
		_driverType = driverTypes[i];;

		hr = D3D11CreateDevice(NULL, _driverType, NULL, uFlag, featureLevels, 
							   numFeatureLevels, D3D11_SDK_VERSION, 
							   &_directDevice11, &_featureLevel, &_directContext);

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) return false;

	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = _directDevice11->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&dxgiDevice);
		if (FAILED(hr)) return false;

		IDXGIAdapter* dxgiAdapter;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&dxgiAdapter);
		if (FAILED(hr)) return false;

		IDXGIFactory* dxgiFactory;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&dxgiFactory);
		if (FAILED(hr)) return false;

		hr = dxgiFactory->CreateSwapChain(_directDevice11, &sd, &_swapChain);
		if (FAILED(hr)) return false;

		dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);

		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(dxgiDevice);
	}

	//백버퍼 생성
	{
		ID3D11Texture2D* backBuffer;
		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr)) return false;

		//랜더 타겟 설정
		hr = _directDevice11->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
		backBuffer->Release();
		if (FAILED(hr)) return false;
	}

	{
		//랜더 타겟 뷰 생성
		_directContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width				= GENERIC::windowWidth;
		depthStencilDesc.Height				= GENERIC::windowHeight;
		depthStencilDesc.MipLevels			= 1;
		depthStencilDesc.ArraySize			= 1;
		depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count	= 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags		= 0;
		depthStencilDesc.MiscFlags			= 0;

		hr = _directDevice11->CreateTexture2D(&depthStencilDesc, 0, &_depthStencilBuffer);
		if (FAILED(hr)) return false;
		hr	= _directDevice11->CreateDepthStencilView(_depthStencilBuffer, 0, &_depthStencilView);
		if (FAILED(hr)) return false;
	}

	_directContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);



	//뷰포트 설정
	_screenViewPort.Width		= GENERIC::windowWidth;
	_screenViewPort.Height		= GENERIC::windowHeight;
	_screenViewPort.MinDepth	= 0;
	_screenViewPort.MaxDepth	= 1;
	_screenViewPort.TopLeftX	= 0;
	_screenViewPort.TopLeftY	= 0;
	_directContext->RSSetViewports(1, &_screenViewPort);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable	= false;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.0f;
	rasterDesc.DepthClipEnable			= true;
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise	= false;
	rasterDesc.MultisampleEnable		= false;
	rasterDesc.ScissorEnable			= false;
	rasterDesc.SlopeScaledDepthBias		= 0.0f;

	// Create the rasterizer state from the description we just filled out.
	//result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	ID3D11RasterizerState* rasterState;
	if (FAILED(_directDevice11->CreateRasterizerState(&rasterDesc, &rasterState)))
	{
		return false;
	}

	_directContext->RSSetState(rasterState);

	if (!InitializeFbx())
	{
		return false;
	}

	//if (!CreateEffectFile("Contents/InstanceBasic.fx", &mShader))
	//{
	//	return false;
	//}
	//
	////mTechnique = mShader->GetTechniqueByName("ColorTech");
	//mTechnique = mShader->GetTechniqueByIndex(0);

	//if (!InitializeVertexLayout())
	//{
	//	return false;
	//}
	return true;
}

bool RenderDevice::InitializeFbx()
{
	_fbxManager = FbxManager::Create();
	
	if (_fbxManager == nullptr)
	{
		return false;
	}

	FbxIOSettings* ios = FbxIOSettings::Create(_fbxManager, IOSROOT);
	_fbxManager->SetIOSettings(ios);
	return true;
}

//bool RenderDevice::InitializeVertexLayout()
//{
//	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0	, 0	, D3D11_INPUT_PER_VERTEX_DATA	, 0	},
//		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0	, 12, D3D11_INPUT_PER_VERTEX_DATA	, 0	},
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0	, 24, D3D11_INPUT_PER_VERTEX_DATA	, 0 },
//		{ "WORLD"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 0	, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
//		{ "WORLD"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 16, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
//		{ "WORLD"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 32, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
//		{ "WORLD"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1 , 64, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
//	};
//
//	D3DX11_PASS_DESC passDesc;
//	
//	/*if (!mTechnique->IsValid())
//	{
//		return false;
//	}*/
//
//	mTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
//
//	if (FAILED(mDirectDevice11->CreateInputLayout(vertexDesc, 7, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
//	{
//		return false;
//	}
//	return true;
//}
//
//bool RenderDevice::CreateEffectFile(char* filepath, LPD3D11EFFECT* effect)
//{
//	return false;
//}

void RenderDevice::Render11()
{
	float color[4] = { 0, 0.125f, 0.3f, 1 };
	_directContext->ClearRenderTargetView(_renderTargetView, color);
	_directContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);


	/*
	Instance용 랜더를 새로 만들어야함.. 메쉬에서 인스탄스 속성을 줘야할듯..
	*/

	//_directContext->IASetInputLayout(_inputLayout);
	_directContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Asynchronous* pAsynchronous = nullptr;

	typedef std::vector<Component::Render*>::iterator listiterator;
	listiterator iter = _listRenders.begin();

	for (Component::Render* render : _listRenders)
	{
		//render->RendMesh();
	}
	
	_swapChain->Present(0, 0);
}

void RenderDevice::LoadAsset()
{
}

void RenderDevice::AddListener(Component::Render* render)
{
	typedef Component::Render Render;

	if (render == nullptr) return;
	
	for (Render* listItem : _listRenders)
	{
		if (listItem != nullptr && listItem == render)
		{
			return;
		}
	}

	_listRenders.push_back(render);
}

void RenderDevice::RemoveListener(Component::Render* render)
{
	if (render == nullptr) return;

	typedef std::vector<Component::Render*>::iterator listiterator;

	listiterator iter = _listRenders.begin();
	while (iter != _listRenders.end())
	{
		Component::Render* elemental = (*iter);
		if (elemental && elemental == render)
		{
			_listRenders.erase(iter);
			break;
		}
		++iter;
	}
}
