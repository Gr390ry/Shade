#include "RenderDevice.h"
#include "General.h"
#include "../Component/Render.h"
#include "../GameObject/Camera.h"
#include <algorithm>

RenderDevice::RenderDevice() : /*mpShader(nullptr), 
							   mpCreateShadowShader(nullptr),
							   mpShadowRenderTarget(nullptr),
							   mpShadowDepthStencil(nullptr),
							   mpFullScreenQuadDecl(nullptr),
							   mpFullScreenQuadVB(nullptr),
							   mpFullScreenQuadIB(nullptr),
							   mpNoEffect(nullptr),
							   mpGrayScale(nullptr),
							   mpSepia(nullptr),
							   mpScreenRenderTarget(nullptr),
							   mpDiffuseTexture(nullptr),
							   mpSpecularTexture(nullptr),
							   mpNormalTexture(nullptr),
							   mpModel(nullptr),*/
							   mFbxManager(nullptr),
							   /*directDevice(nullptr),
							   directObject(nullptr),*/
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

	//SAFE_RELEASE(mpShader);
	//SAFE_RELEASE(mpCreateShadowShader);
	//SAFE_RELEASE(mpShadowRenderTarget);
	//SAFE_RELEASE(mpShadowDepthStencil);

	//SAFE_RELEASE(mpFullScreenQuadDecl);
	//SAFE_RELEASE(mpFullScreenQuadVB);
	//SAFE_RELEASE(mpFullScreenQuadIB);

	//SAFE_RELEASE(mpNoEffect);
	//SAFE_RELEASE(mpGrayScale);
	//SAFE_RELEASE(mpSepia);
	//SAFE_RELEASE(mpScreenRenderTarget);

	//SAFE_RELEASE(mpDiffuseTexture);
	//SAFE_RELEASE(mpSpecularTexture);
	//SAFE_RELEASE(mpNormalTexture);
	//SAFE_RELEASE(mpModel);
	//SAFE_RELEASE(directDevice);
	//SAFE_RELEASE(directObject);

	SAFE_RELEASE(mShader);
	SAFE_RELEASE(mRenderTargetView);
	SAFE_RELEASE(mDepthStencilBuffer);
	SAFE_RELEASE(mSwapChain);
	SAFE_RELEASE(mDirectContext);
	SAFE_RELEASE(mDirectDevice11);
}

//bool RenderDevice::InitializeDevice(HWND hWnd)
//{
//	directObject = Direct3DCreate9(D3D_SDK_VERSION);
//
//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory(&d3dpp, sizeof(d3dpp));
//
//	d3dpp.BackBufferWidth			= GENERIC::windowWidth;
//	d3dpp.BackBufferHeight			= GENERIC::windowHeight;
//	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
//	d3dpp.BackBufferCount			= 0;
//	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
//	d3dpp.MultiSampleQuality		= 0;
//	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
//	d3dpp.hDeviceWindow				= hWnd;
//	d3dpp.EnableAutoDepthStencil	= TRUE;
//	d3dpp.Windowed					= TRUE;
//	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24X8;
//	d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
//	d3dpp.FullScreen_RefreshRateInHz = 0;
//	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;
//
//	if (FAILED(directObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
//										  D3DCREATE_HARDWARE_VERTEXPROCESSING, 
//										  &d3dpp, &directDevice)))
//	{
//		OutputDebugString("Create Device Failed!");
//		return false;
//	}
//
//	if (!InitializeFbx())
//	{
//		return false;
//	}
//
//	if (!InitializeBuffer())
//	{
//		OutputDebugString("Initialize Buffer Failed!");
//		return false;
//	}
//
//	if (!InitializeFullScreenQuad())
//	{
//		return false;
//	}
//
//	listRenders.clear();
//	return true;
//}

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

	if (!CreateEffectFile("Contents/color.fx", &mShader))
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
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	
	/*if (!mTechnique->IsValid())
	{
		return false;
	}*/

	mTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	if (FAILED(mDirectDevice11->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
	{
		return false;
	}
	return true;
}

bool RenderDevice::CreateEffectFile(char* filepath, LPD3D11EFFECT* effect)
{
	DWORD shaderFlag = 0;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlag |= D3D10_SHADER_DEBUG;
	shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMsgs = nullptr;

	//effect 파일 컴파일
	if (FAILED(D3DX11CompileFromFile(filepath, 0, 0, 0, "fx_5_0", 
									 shaderFlag, 0, 0, 
									 &compiledShader, &compilationMsgs, 0)))
	{
		if (compilationMsgs != 0)
		{
			MessageBoxA(NULL, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
			SAFE_RELEASE(compilationMsgs);
		}
		assert(NULL);
		return false;
	}
	
	/* - NOTICE -
		컴파일이 완료된 쉐이더를 이펙트 파일로 만든다.
	*/
	HRESULT hr;

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),
									  compiledShader->GetBufferSize(),
									  0, mDirectDevice11, effect);
	SAFE_RELEASE(compiledShader);

	if (FAILED(hr)) return false;
	return true;
}

//bool RenderDevice::InitializeBuffer()
//{
//	if (FAILED(directDevice->CreateTexture(GENERIC::shadowMapSize, GENERIC::shadowMapSize, 1, D3DUSAGE_RENDERTARGET,
//		D3DFMT_R32F, D3DPOOL_DEFAULT, &mpShadowRenderTarget, NULL)))
//	{
//		return false;
//	}
//	if (FAILED(directDevice->CreateDepthStencilSurface(GENERIC::shadowMapSize, GENERIC::shadowMapSize,
//		D3DFMT_D24X8, D3DMULTISAMPLE_NONE,
//		0, TRUE, &mpShadowDepthStencil, NULL)))
//	{
//		return false;
//	}
//
//	return true;
//}

//bool RenderDevice::InitializeFullScreenQuad()
//{
//	D3DVERTEXELEMENT9 vtxDesc[3];
//
//	int offset = 0;
//	int i = 0;
//
//	vtxDesc[i].Stream = 0;
//	vtxDesc[i].Offset = offset;
//	vtxDesc[i].Type = D3DDECLTYPE_FLOAT3;
//	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
//	vtxDesc[i].Usage = D3DDECLUSAGE_POSITION;
//	vtxDesc[i].UsageIndex = 0;
//
//	offset += sizeof(float) * 3;
//	++i;
//
//	vtxDesc[i].Stream = 0;
//	vtxDesc[i].Offset = offset;
//	vtxDesc[i].Type = D3DDECLTYPE_FLOAT2;
//	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
//	vtxDesc[i].Usage = D3DDECLUSAGE_TEXCOORD;
//	vtxDesc[i].UsageIndex = 0;
//
//	offset += sizeof(float) * 3;
//	++i;
//
//	vtxDesc[i].Stream = 0xFF;//D3DDECL_END();
//	vtxDesc[i].Offset = 0;
//	vtxDesc[i].Type = D3DDECLTYPE_UNUSED;
//	vtxDesc[i].Method = 0;
//	vtxDesc[i].Usage = 0;
//	vtxDesc[i].UsageIndex = 0;
//
//
//	if (FAILED(directDevice->CreateVertexDeclaration(vtxDesc, &mpFullScreenQuadDecl)))
//	{
//		return false;
//	}
//
//	directDevice->CreateVertexBuffer(offset * 4, 0, 0, D3DPOOL_MANAGED, &mpFullScreenQuadVB, nullptr);
//
//	void* vertexData = nullptr;
//
//	mpFullScreenQuadVB->Lock(0, 0, &vertexData, 0); //정점 좌표 및 UV 정보 입력
//	{
//		float* data = (float*)vertexData;
//
//		*data++ = -1;	*data++ = 1;	*data++ = 0;	//정점 좌표
//		*data++ = 0;	*data++ = 0;					//UV
//		*data++ = 1;	*data++ = 1;	*data++ = 0;	
//
//		*data++ = 1;	*data++ = 0;
//		*data++ = 1;	*data++ = -1;	*data++ = 0;	
//
//		*data++ = 1;	*data++ = 1;
//		*data++ = -1;	*data++ = -1;	*data++ = 0;	*data++ = 0;	*data++ = 1;
//	}
//	mpFullScreenQuadVB->Unlock();
//
//	directDevice->CreateIndexBuffer(sizeof(short) * 6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mpFullScreenQuadIB, nullptr);
//
//	void* indexData;
//	mpFullScreenQuadIB->Lock(0, 0, &indexData, 0); //삼각형 인덱스 입력
//	{
//		unsigned short* data = (unsigned short*)indexData;
//		*data++ = 0;	*data++ = 1;	*data++ = 3;
//		*data++ = 3;	*data++ = 1;	*data++ = 2;
//	}
//	mpFullScreenQuadIB->Unlock();
//
//	if (FAILED(directDevice->CreateTexture(GENERIC::windowWidth,GENERIC::windowHeight,
//											1, D3DUSAGE_RENDERTARGET, 
//											D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, 
//											&mpScreenRenderTarget, NULL)))
//	{
//		return false;
//	}
//
//	return true;
//}

void RenderDevice::Render11()
{
	float color[4] = { 0, 0.125f, 0.3f, 1 };
	mDirectContext->ClearRenderTargetView(mRenderTargetView, color);
	mDirectContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	mDirectContext->IASetInputLayout(mInputLayout);
	mDirectContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	typedef std::vector<Component::Render*>::iterator listiterator;
	listiterator iter = listRenders.begin();
	while (iter != listRenders.end())
	{
		Component::Render* elemental = (*iter);
		elemental->DrawFbxMesh(mTechnique);
		++iter;
	}


	mSwapChain->Present(0, 0);
}

//void RenderDevice::RenderFrame()
//{
//	directDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.f, 0);
//
//	directDevice->BeginScene();
//	{
//		D3DXMATRIXA16 matWorld;
//		D3DXMatrixIdentity(&matWorld);
//
//		static float fYaw = 0.f;
//		
//		fYaw += 0.4f * GENERIC::PI / 180.f;
//		D3DXMatrixRotationY(&matWorld, fYaw);
//
//		Draw(matWorld, General::Get()->GetMainCamera()->GetViewMarix(), General::Get()->GetMainCamera()->GetProjectionMatrix());
//	}
//	directDevice->EndScene();
//
//	directDevice->Present(NULL, NULL, NULL, NULL);
//}

void RenderDevice::LoadAsset()
{
	//mpShader				= ConstructHelper::LoadShader("Contents/Fx/ColorConversion.fx");
	//mpCreateShadowShader	= ConstructHelper::LoadShader("Contents/Fx/CreateShadowShader.fx");
	//mpDiffuseTexture		= ConstructHelper::LoadTexture("Contents/Texture/Fieldstone_DM.tga");
	//mpSpecularTexture		= ConstructHelper::LoadTexture("Contents/Texture/fieldstone_SM.tga");
	//mpNormalTexture			= ConstructHelper::LoadTexture("Contents/Texture/fieldstone_NM.tga");
	//mpModel					= ConstructHelper::LoadXMesh("Contents/Model/Torus.x");
	//mpNoEffect				= ConstructHelper::LoadShader("Contents/Fx/NoEffect.fx");
	//mpGrayScale				= ConstructHelper::LoadShader("Contents/Fx/GrayScale.fx");
	//mpSepia					= ConstructHelper::LoadShader("Contents/Fx/Sepia.fx");
}

//void RenderDevice::RenderTarget(const LPD3DXEFFECT& fx, const LPD3DXMESH& mesh, const DWORD& flags = 0, const DWORD& meshAttribute = 0)
//{
//	UINT numPasses = 0U;
//	fx->Begin(&numPasses, flags);
//	{
//		for (UINT i = 0U; i < numPasses; ++i)
//		{
//			fx->BeginPass(i);
//			{
//				mesh->DrawSubset(meshAttribute);
//			}
//			fx->EndPass();
//		}
//	}
//	fx->End();
//}

//void RenderDevice::DrawStencilBuffer()
//{
//	LPDIRECT3DSURFACE9 pHWBackBuffer = nullptr;
//	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = nullptr;
//	directDevice->GetRenderTarget(0, &pHWBackBuffer);
//	directDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);
//
//	LPDIRECT3DSURFACE9 pShadowSurface = nullptr;
//	if (SUCCEEDED(mpShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
//	{
//		directDevice->SetRenderTarget(0, pShadowSurface);
//		pShadowSurface->Release();
//		pShadowSurface = nullptr;
//	}
//	directDevice->SetDepthStencilSurface(mpShadowDepthStencil);
//
//	directDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);
//
//	{
//		UINT numPasses = 0;
//		mpCreateShadowShader->Begin(&numPasses, NULL);
//		{
//			for (UINT i = 0; i < numPasses; ++i)
//			{
//				mpCreateShadowShader->BeginPass(i);
//				{
//					mpModel->DrawSubset(0);
//				}
//				mpCreateShadowShader->EndPass();
//			}
//		}
//		mpCreateShadowShader->End();
//	}
//
//	directDevice->SetRenderTarget(0, pHWBackBuffer);
//	directDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);
//
//	pHWBackBuffer->Release();
//	pHWBackBuffer = nullptr;
//	pHWDepthStencilBuffer->Release();
//	pHWDepthStencilBuffer = nullptr;
//}

//void RenderDevice::Draw(const Matrix4x4& matWorld, const Matrix4x4& matView, const Matrix4x4& matProjection)
//{
//	Matrix4x4 matViewProjection;
//	D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
//	
//	//포스트 이펙트 용 랜더 타겟 설정.
//	LPDIRECT3DSURFACE9 pHWBackBuffer = nullptr;
//	directDevice->GetRenderTarget(0, &pHWBackBuffer);
//
//	LPDIRECT3DSURFACE9 pSceneSurface = nullptr;
//
//	if (SUCCEEDED(mpScreenRenderTarget->GetSurfaceLevel(0, &pSceneSurface)))
//	{
//		directDevice->SetRenderTarget(0, pSceneSurface);
//		pSceneSurface->Release();
//		pSceneSurface = nullptr;
//	}
//
//	directDevice->Clear(0, nullptr, D3DCLEAR_TARGET, 0XFF000000, 1, 0);
//	
//
//	
//
//	mpShader->SetMatrix("WorldMatrix", &matWorld);
//	mpShader->SetMatrix("ViewProjectionMatrix", &matViewProjection);
//	mpShader->SetVector("WorldLightPosition", &GENERIC::worldLightPosition);
//	mpShader->SetVector("WorldCameraPosition", &GENERIC::worldCameraPosition);
//	mpShader->SetTexture("DiffuseMap_Tex", mpDiffuseTexture);
//
//	
//	UINT numPasses = 0;
//	mpShader->Begin(&numPasses, 0);
//	{
//		for (UINT i = 0; i < numPasses; ++i)
//		{
//			mpShader->BeginPass(i);
//			{
//				mpModel->DrawSubset(0);
//
//				typedef std::vector<Component::Render*>::iterator listiterator;
//				listiterator iter = listRenders.begin();
//				while (iter != listRenders.end())
//				{
//					Component::Render* elemental = (*iter);
//					elemental->DrawOnScreen(mpShader);
//					++iter;
//				}
//			}
//			mpShader->EndPass();
//		}
//	}
//	mpShader->End();
//
//	//포스트이펙트 처리 부분
//
//	directDevice->SetRenderTarget(0, pHWBackBuffer);
//	pHWBackBuffer->Release();
//	pHWBackBuffer = nullptr;
//	
//	LPD3DXEFFECT effectToUse = mpNoEffect;
//	effectToUse->SetTexture("Scene_Tex", mpScreenRenderTarget);
//	effectToUse->Begin(&numPasses, 0);
//	{
//		for (UINT i = 0; i < numPasses; ++i)
//		{ 
//			effectToUse->BeginPass(i);
//			{
//				directDevice->SetStreamSource(0, mpFullScreenQuadVB, 0, sizeof(float) * 5);
//				directDevice->SetIndices(mpFullScreenQuadIB);
//				directDevice->SetVertexDeclaration(mpFullScreenQuadDecl);
//
//				directDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
//
//			}
//			effectToUse->EndPass();
//		}
//	}
//	effectToUse->End();
//	
//}

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



//const LPDIRECT3DDEVICE9 RenderDevice::GetDevice()
//{
//	return directDevice;
//}