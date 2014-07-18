#include "RenderDevice.h"
#include "General.h"
#include "../Component/Render.h"
#include "../GameObject/Camera.h"
#include <algorithm>

RenderDevice::RenderDevice() : mFbxManager(nullptr),
							   mInstanceBuffer(nullptr),
							   mDirectDevice11(nullptr),
							   mDirectContext(nullptr),
							   mSwapChain(nullptr),
							   mDepthStencilBuffer(nullptr),
							   mRenderTargetView(nullptr)
{
}

void RenderDevice::Release()
{
	if (mFbxManager)
	{
		mFbxManager->Destroy();
	}

	SAFE_RELEASE(mInstanceBuffer);
	SAFE_RELEASE(mShader);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mDirectContext);
	SAFE_RELEASE(mDirectDevice11);
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
		mDriverType = driverTypes[i];;

		hr = D3D11CreateDevice(NULL, mDriverType, NULL, uFlag, featureLevels, 
							   numFeatureLevels, D3D11_SDK_VERSION, 
							   &mDirectDevice11, &mFeatureLevel, &mDirectContext);

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) return false;

	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = mDirectDevice11->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&dxgiDevice);
		if (FAILED(hr)) return false;

		IDXGIAdapter* dxgiAdapter;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&dxgiAdapter);
		if (FAILED(hr)) return false;

		IDXGIFactory* dxgiFactory;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&dxgiFactory);
		if (FAILED(hr)) return false;

		hr = dxgiFactory->CreateSwapChain(mDirectDevice11, &sd, &mSwapChain);
		if (FAILED(hr)) return false;

		dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);

		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(dxgiDevice);
	}

	//백버퍼 생성
	{
		ID3D11Texture2D* backBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr)) return false;

		//랜더 타겟 설정
		hr = mDirectDevice11->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
		backBuffer->Release();
		if (FAILED(hr)) return false;
	}

	{
		//랜더 타겟 뷰 생성
		mDirectContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = GENERIC::windowWidth;
		depthStencilDesc.Height = GENERIC::windowHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		hr = mDirectDevice11->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
		if (FAILED(hr)) return false;
		hr = mDirectDevice11->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
		if (FAILED(hr)) return false;
	}

	mDirectContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);



	//뷰포트 설정
	mScreenViewPort.Width = GENERIC::windowWidth;
	mScreenViewPort.Height = GENERIC::windowHeight;
	mScreenViewPort.MinDepth = 0;
	mScreenViewPort.MaxDepth = 1;
	mScreenViewPort.TopLeftX = 0;
	mScreenViewPort.TopLeftY = 0;
	mDirectContext->RSSetViewports(1, &mScreenViewPort);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	//result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	ID3D11RasterizerState* rasterState;
	if (FAILED(mDirectDevice11->CreateRasterizerState(&rasterDesc, &rasterState)))
	{
		return false;
	}

	mDirectContext->RSSetState(rasterState);

	if (!InitializeFbx())
	{
		return false;
	}

	if (!CreateEffectFile("Contents/InstanceBasic.fx", &mShader))
	{
		return false;
	}
	//mTechnique = mShader->GetTechniqueByName("ColorTech");
	mTechnique = mShader->GetTechniqueByIndex(0);

	if (!InitializeVertexLayout())
	{
		return false;
	}
	return true;
}

bool RenderDevice::InitializeFbx()
{
	mFbxManager = FbxManager::Create();
	
	if (mFbxManager == nullptr)
	{
		return false;
	}

	FbxIOSettings* ios = FbxIOSettings::Create(mFbxManager, IOSROOT);
	mFbxManager->SetIOSettings(ios);
	return true;
}

bool RenderDevice::InitializeVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0	, 0	, D3D11_INPUT_PER_VERTEX_DATA	, 0	},
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0	, 12, D3D11_INPUT_PER_VERTEX_DATA	, 0	},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT		, 0	, 24, D3D11_INPUT_PER_VERTEX_DATA	, 0 },
		{ "WORLD"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 0	, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
		{ "WORLD"	, 1, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 16, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
		{ "WORLD"	, 2, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1	, 32, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
		{ "WORLD"	, 3, DXGI_FORMAT_R32G32B32A32_FLOAT	, 1 , 64, D3D11_INPUT_PER_INSTANCE_DATA	, 1 }, //인스탄스 설정
	};

	D3DX11_PASS_DESC passDesc;
	
	/*if (!mTechnique->IsValid())
	{
		return false;
	}*/

	mTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	if (FAILED(mDirectDevice11->CreateInputLayout(vertexDesc, 7, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
	{
		return false;
	}
	return true;
}

bool RenderDevice::CreateEffectFile(char* filepath, LPD3D11EFFECT* effect)
{
	return false;
}

void RenderDevice::Render11()
{
	float color[4] = { 0, 0.125f, 0.3f, 1 };
	mDirectContext->ClearRenderTargetView(mRenderTargetView, color);
	mDirectContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);


	/*
	Instance용 랜더를 새로 만들어야함.. 메쉬에서 인스탄스 속성을 줘야할듯..
	*/

	mDirectContext->IASetInputLayout(mInputLayout);
	mDirectContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11Asynchronous* pAsynchronous = nullptr;


	/*HRESULT queryResult = mDirectContext->GetData(pAsynchronous, nullptr, 0, 0);
	if (SUCCEEDED(queryResult))
	{
		mDirectContext->Begin(pAsynchronous);
	}	*/
	
	typedef std::vector<Component::Render*>::iterator listiterator;
	listiterator iter = listRenders.begin();
	while (iter != listRenders.end())
	{
		Component::Render* elemental = (*iter);
		elemental->RendMesh(mTechnique);
		++iter;
	}

	/*if (SUCCEEDED(queryResult))
	{
		mDirectContext->End(pAsynchronous);
	}*/

	mSwapChain->Present(0, 0);
}

void RenderDevice::LoadAsset()
{
}

void RenderDevice::AddListener(Component::Render* pRender)
{
	if (pRender == nullptr) return;

	typedef std::vector<Component::Render*>::iterator listiterator;

	listiterator iter = listRenders.begin();
	while (iter != listRenders.end())
	{
		Component::Render* elemental = (*iter);
		if (elemental && elemental == pRender)
		{
			return;
		}
		++iter;
	}
	listRenders.push_back(pRender);
}

void RenderDevice::RemoveListener(Component::Render* pRender)
{
	if (pRender == nullptr) return;

	typedef std::vector<Component::Render*>::iterator listiterator;

	listiterator iter = listRenders.begin();
	while (iter != listRenders.end())
	{
		Component::Render* elemental = (*iter);
		if (elemental && elemental == pRender)
		{
			listRenders.erase(iter);
			break;
		}
		++iter;
	}
}
