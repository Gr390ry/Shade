#include "RenderDevice.h"
#include "General.h"
#include "MeshPool.h"
#include "../Component/Render.h"
#include "../GameObject/Camera.h"
#include "../GameObject/Actor.h"
#include "../IncludeAssets.h"

#include <algorithm>

RenderDevice::RenderDevice() : _fbxManager(nullptr),
							   _instancedBuffer(nullptr),
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

	SAFE_RELEASE(_instancedBuffer);
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
	rasterDesc.CullMode					= D3D11_CULL_NONE;
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

void RenderDevice::Render11()
{
	float color[4] = { 0, 0.125f, 0.3f, 1 };
	ID3D11InputLayout* inputLayout		= Render::Effect::InstancedBasic::Get()->GetLayout();
	ID3DX11EffectTechnique* activeTech	= Render::Effect::InstancedBasic::Get()->GetTechique();	
	IMesh* mesh		= MeshPool::Get()->GetMeshData("Box");

	assert(inputLayout != nullptr);
	assert(activeTech != nullptr);
	assert(mesh != nullptr);

	_directContext->ClearRenderTargetView(_renderTargetView, color);
	_directContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	
	_directContext->IASetInputLayout(inputLayout);
	_directContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT strides[2] = { sizeof(GENERIC::Vertex), sizeof(GENERIC::InstancedData) };
	UINT offsets[2]	= { 0, 0 };

	ID3D11Buffer* vertexBuffers[2] = { mesh->GetVB(), _instancedBuffer };

	XMMATRIX view			= General::Get()->GetMainCamera()->GetViewMarix();
	XMMATRIX projection		= General::Get()->GetMainCamera()->GetProjectionMatrix();
	XMMATRIX vp				= XMMatrixMultiply(view, projection);
	XMFLOAT4 lightPosition	= XMFLOAT4(500, 500, -500, 1);
	XMVECTOR lightVector	= XMLoadFloat4(&lightPosition);

	Render::Effect::InstancedBasic::Get()->SetViewProjectionMatrix(&vp);

	LPD3D11EFFECT effect	= Render::Effect::InstancedBasic::Get()->GetFx();
	ID3DX11EffectMatrixVariable*	viewProjection = effect->GetVariableByName("gViewProjection")->AsMatrix();

	//Render::Effect::InstancedBasic::Get()->SetLightDirection(&lightVector);

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		_directContext->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
		_directContext->IASetIndexBuffer(mesh->GetIB(), DXGI_FORMAT_R32_UINT, 0);

		//Render::Effect::InstancedBasic::Get()->SetWorldMatrix(&XMMatrixIdentity());	
		viewProjection->SetMatrix(reinterpret_cast<float*>(&vp));

		activeTech->GetPassByIndex(p)->Apply(0, _directContext);
		_directContext->DrawIndexedInstanced(mesh->GetNumIndices(), _vecInstancedData.size(), 0, 0, 0);
	}
	_swapChain->Present(0, 0);
}

void RenderDevice::UpdateScene(const float& delta)
{
	//D3D11_MAPPED_SUBRESOURCE mappedData;
	//_directContext->Map(_instancedBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	//GENERIC::InstancedData* dataView = reinterpret_cast<GENERIC::InstancedData*>(mappedData.pData);

	//for (UINT i = 0; i < _vecInstancedData.size(); ++i)
	//{
	//	dataView[i] = _vecInstancedData[i];
	//}

	//_directContext->Unmap(_instancedBuffer, 0);
}

void RenderDevice::DrawScene()
{
}

void RenderDevice::LoadAsset()
{
}

void RenderDevice::BuildInstancedBuffer()
{
	/*_vecInstancedData.clear();
	for (XMMATRIX worldMatrix : General::Get()->GetInstancedObjectsWorldMatrix())
	{
		GENERIC::InstancedData instancedData;

		instancedData._world = worldMatrix;
		_vecInstancedData.emplace_back(instancedData);
	}*/

	const int n = 5;
	_vecInstancedData.resize(n*n*n);

	float width = 200.0f;
	float height = 200.0f;
	float depth = 200.0f;

	float x = -0.5f*width;
	float y = -0.5f*height;
	float z = -0.5f*depth;
	float dx = width / (n - 1);
	float dy = height / (n - 1);
	float dz = depth / (n - 1);
	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				// Position instanced along a 3D grid.
				_vecInstancedData[k*n*n + i*n + j]._world = XMFLOAT4X4(
					100.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 100.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 100.0f, 0.0f,
					x + j*dx, y + i*dy, z + k*dz, 1.0f);

				// Random color.
				_vecInstancedData[k*n*n + i*n + j]._color.x = 1.f;
				_vecInstancedData[k*n*n + i*n + j]._color.y = 1.f;
				_vecInstancedData[k*n*n + i*n + j]._color.z = 1.f;
				_vecInstancedData[k*n*n + i*n + j]._color.w = 1.0f;
			}
		}
	}

	D3D11_BUFFER_DESC instancedBufferDesc;
	instancedBufferDesc.Usage		= D3D11_USAGE_DYNAMIC;
	instancedBufferDesc.ByteWidth	= sizeof(GENERIC::InstancedData) * _vecInstancedData.size();
	instancedBufferDesc.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
	instancedBufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	instancedBufferDesc.MiscFlags	= 0;
	instancedBufferDesc.StructureByteStride	= 0;

	if (FAILED(_directDevice11->CreateBuffer(&instancedBufferDesc, 0, &_instancedBuffer)))
	{
		assert(false);
	}
	//_instanceBuffer;
}

void RenderDevice::AddListener(Component::Render* render)
{
	typedef Component::Render Render;

	if (render == nullptr) return;
	
	for (Render* listItem : _vecRenders)
	{
		if (listItem != nullptr && listItem == render)
		{
			return;
		}
	}

	_vecRenders.push_back(render);
}

void RenderDevice::RemoveListener(Component::Render* render)
{
	if (render == nullptr) return;

	typedef std::vector<Component::Render*>::iterator listiterator;

	listiterator iter = _vecRenders.begin();
	while (iter != _vecRenders.end())
	{
		Component::Render* elemental = (*iter);
		if (elemental && elemental == render)
		{
			_vecRenders.erase(iter);
			break;
		}
		++iter;
	}
}
