#include "ShadeEngine.h"
#include "RenderDevice.h"
#include "Component\Render.h"
#include <algorithm>

void RenderDevice::Release()
{
	//SAFE_RELEASE();

	SAFE_RELEASE(mpShader);
	SAFE_RELEASE(mpCreateShadowShader);
	SAFE_RELEASE(mpShadowRenderTarget);
	SAFE_RELEASE(mpShadowDepthStencil);

	SAFE_RELEASE(mpFullScreenQuadDecl);
	SAFE_RELEASE(mpFullScreenQuadVB);
	SAFE_RELEASE(mpFullScreenQuadIB);

	SAFE_RELEASE(mpNoEffect);
	SAFE_RELEASE(mpGrayScale);
	SAFE_RELEASE(mpSepia);
	SAFE_RELEASE(mpScreenRenderTarget);

	SAFE_RELEASE(mpDiffuseTexture);
	SAFE_RELEASE(mpSpecularTexture);
	SAFE_RELEASE(mpNormalTexture);
	SAFE_RELEASE(mpModel);
	SAFE_RELEASE(mpDisc);
	SAFE_RELEASE(directDevice);
	SAFE_RELEASE(directObject);
}

bool RenderDevice::InitializeDevice(HWND hWnd)
{
	directObject = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth			= GENERIC::windowWidth;
	d3dpp.BackBufferHeight			= GENERIC::windowHeight;
	d3dpp.BackBufferFormat			= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount			= 0;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				= hWnd;
	d3dpp.EnableAutoDepthStencil	= TRUE;
	d3dpp.Windowed					= TRUE;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24X8;
	d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_ONE;

	if (FAILED(directObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
										  D3DCREATE_HARDWARE_VERTEXPROCESSING, 
										  &d3dpp, &directDevice)))
	{
		OutputDebugString("Create Device Failed!");
		return false;
	}


	if (!InitializeBuffer())
	{
		OutputDebugString("Initialize Buffer Failed!");
		return false;
	}

	if (!InitializeFullScreenQuad())
	{
		return false;
	}

	listRenders.clear();
	return true;
}

bool RenderDevice::InitializeBuffer()
{
	if (FAILED(directDevice->CreateTexture(GENERIC::shadowMapSize, GENERIC::shadowMapSize, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F, D3DPOOL_DEFAULT, &mpShadowRenderTarget, NULL)))
	{
		return false;
	}
	if (FAILED(directDevice->CreateDepthStencilSurface(GENERIC::shadowMapSize, GENERIC::shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE,
		0, TRUE, &mpShadowDepthStencil, NULL)))
	{
		return false;
	}

	return true;
}

bool RenderDevice::InitializeFullScreenQuad()
{
	D3DVERTEXELEMENT9 vtxDesc[3];

	int offset = 0;
	int i = 0;

	vtxDesc[i].Stream = 0;
	vtxDesc[i].Offset = offset;
	vtxDesc[i].Type = D3DDECLTYPE_FLOAT3;
	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage = D3DDECLUSAGE_POSITION;
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 3;
	++i;

	vtxDesc[i].Stream = 0;
	vtxDesc[i].Offset = offset;
	vtxDesc[i].Type = D3DDECLTYPE_FLOAT2;
	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage = D3DDECLUSAGE_TEXCOORD;
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 3;
	++i;

	vtxDesc[i].Stream = 0xFF;//D3DDECL_END();
	vtxDesc[i].Offset = 0;
	vtxDesc[i].Type = D3DDECLTYPE_UNUSED;
	vtxDesc[i].Method = 0;
	vtxDesc[i].Usage = 0;
	vtxDesc[i].UsageIndex = 0;


	if (FAILED(directDevice->CreateVertexDeclaration(vtxDesc, &mpFullScreenQuadDecl)))
	{
		return false;
	}

	directDevice->CreateVertexBuffer(offset * 4, 0, 0, D3DPOOL_MANAGED, &mpFullScreenQuadVB, nullptr);

	void* vertexData = nullptr;

	mpFullScreenQuadVB->Lock(0, 0, &vertexData, 0); //정점 좌표 및 UV 정보 입력
	{
		float* data = (float*)vertexData;

		*data++ = -1;	*data++ = 1;	*data++ = 0;	//정점 좌표
		*data++ = 0;	*data++ = 0;					//UV
		*data++ = 1;	*data++ = 1;	*data++ = 0;	

		*data++ = 1;	*data++ = 0;
		*data++ = 1;	*data++ = -1;	*data++ = 0;	

		*data++ = 1;	*data++ = 1;
		*data++ = -1;	*data++ = -1;	*data++ = 0;	*data++ = 0;	*data++ = 1;
	}
	mpFullScreenQuadVB->Unlock();

	directDevice->CreateIndexBuffer(sizeof(short) * 6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mpFullScreenQuadIB, nullptr);

	void* indexData;
	mpFullScreenQuadIB->Lock(0, 0, &indexData, 0); //삼각형 인덱스 입력
	{
		unsigned short* data = (unsigned short*)indexData;
		*data++ = 0;	*data++ = 1;	*data++ = 3;
		*data++ = 3;	*data++ = 1;	*data++ = 2;
	}
	mpFullScreenQuadIB->Unlock();

	if (FAILED(directDevice->CreateTexture(GENERIC::windowWidth,GENERIC::windowHeight,
											1, D3DUSAGE_RENDERTARGET, 
											D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, 
											&mpScreenRenderTarget, NULL)))
	{
		return false;
	}

	return true;
}

void RenderDevice::RenderFrame()
{
	directDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000, 1.f, 0);

	directDevice->BeginScene();
	{
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEye(GENERIC::worldCameraPosition.x, GENERIC::worldCameraPosition.y, GENERIC::worldCameraPosition.z);
		D3DXVECTOR3 vLook(0, 0, 0);
		D3DXVECTOR3 vUp(0, 1, 0);
		D3DXMatrixLookAtLH(&matView, &vEye, &vLook, &vUp);

		D3DXMATRIXA16 matProjection;
		D3DXMatrixPerspectiveFovLH(&matProjection, GENERIC::FOV, GENERIC::ASPECT_RATIO, GENERIC::nearPlane, GENERIC::farPlane);

		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);

		static float fYaw = 0.f;
		
		fYaw += 0.4f * GENERIC::PI / 180.f;
		D3DXMatrixRotationY(&matWorld, fYaw);

		Draw(matWorld, matView, matProjection);
	}
	directDevice->EndScene();

	directDevice->Present(NULL, NULL, NULL, NULL);
}

void RenderDevice::LoadAsset()
{
	mpShader = ConstructHelper::LoadShader("Contents/Fx/ColorConversion.fx");
	mpCreateShadowShader = ConstructHelper::LoadShader("Contents/Fx/CreateShadowShader.fx");
	mpDiffuseTexture = ConstructHelper::LoadTexture("Contents/Texture/Fieldstone_DM.tga");
	mpSpecularTexture = ConstructHelper::LoadTexture("Contents/Texture/fieldstone_SM.tga");
	mpNormalTexture = ConstructHelper::LoadTexture("Contents/Texture/fieldstone_NM.tga");
	mpModel = ConstructHelper::LoadXMesh("Contents/Model/Torus.x");
	mpDisc = ConstructHelper::LoadXMesh("Contents/Model/Disc.x");

	mpNoEffect = ConstructHelper::LoadShader("Contents/Fx/NoEffect.fx");
	mpGrayScale = ConstructHelper::LoadShader("Contents/Fx/GrayScale.fx");
	mpSepia = ConstructHelper::LoadShader("Contents/Fx/Sepia.fx");
}

void RenderDevice::RenderTarget(const LPD3DXEFFECT& fx, const LPD3DXMESH& mesh, const DWORD& flags = 0, const DWORD& meshAttribute = 0)
{
	UINT numPasses = 0U;
	fx->Begin(&numPasses, flags);
	{
		for (UINT i = 0U; i < numPasses; ++i)
		{
			fx->BeginPass(i);
			{
				mesh->DrawSubset(meshAttribute);
			}
			fx->EndPass();
		}
	}
	fx->End();
}

void RenderDevice::DrawStencilBuffer()
{
	LPDIRECT3DSURFACE9 pHWBackBuffer = nullptr;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = nullptr;
	directDevice->GetRenderTarget(0, &pHWBackBuffer);
	directDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	LPDIRECT3DSURFACE9 pShadowSurface = nullptr;
	if (SUCCEEDED(mpShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		directDevice->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = nullptr;
	}
	directDevice->SetDepthStencilSurface(mpShadowDepthStencil);

	directDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	{
		UINT numPasses = 0;
		mpCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				mpCreateShadowShader->BeginPass(i);
				{
					mpModel->DrawSubset(0);
				}
				mpCreateShadowShader->EndPass();
			}
		}
		mpCreateShadowShader->End();
	}

	directDevice->SetRenderTarget(0, pHWBackBuffer);
	directDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = nullptr;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = nullptr;
}

void RenderDevice::Draw(const Matrix4x4& matWorld, const Matrix4x4& matView, const Matrix4x4& matProjection)
{
	/*Matrix4x4 matLightView;
	Matrix4x4 matLightProjection;
	Matrix4x4 matViewProjection;

	Vector3 vEye(GENERIC::worldLightPosition.x, GENERIC::worldLightPosition.y, GENERIC::worldLightPosition.z);
	Vector3 vLook(0, 0, 0);
	Vector3 vUp(0, 1, 0);
	D3DXMatrixLookAtLH(&matLightView, &vEye, &vLook, &vUp);
	D3DXMatrixPerspectiveFovLH(&matLightProjection, GENERIC::FOV, GENERIC::ASPECT_RATIO, GENERIC::nearPlane, GENERIC::farPlane);
	D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);*/

	Matrix4x4 matViewProjection;
	D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);

	/*{
		mpCreateShadowShader->SetMatrix("WorldMatrix", &matWorld);
		mpCreateShadowShader->SetMatrix("LightViewMatrix", &matLightView);
		mpCreateShadowShader->SetMatrix("LightProjectionMatrix", &matLightProjection);
		DrawStencilBuffer();
	}*/

	//포스트 이펙트 용 랜더 타겟 설정.
	LPDIRECT3DSURFACE9 pHWBackBuffer = nullptr;
	directDevice->GetRenderTarget(0, &pHWBackBuffer);

	LPDIRECT3DSURFACE9 pSceneSurface = nullptr;

	if (SUCCEEDED(mpScreenRenderTarget->GetSurfaceLevel(0, &pSceneSurface)))
	{
		directDevice->SetRenderTarget(0, pSceneSurface);
		pSceneSurface->Release();
		pSceneSurface = nullptr;
	}

	directDevice->Clear(0, nullptr, D3DCLEAR_TARGET, 0XFF000000, 1, 0);
	

	

	mpShader->SetMatrix("WorldMatrix", &matWorld);
	mpShader->SetMatrix("ViewProjectionMatrix", &matViewProjection);
	//mpShader->SetMatrix("ViewProjectionMatrix", &matViewProjection);
	/*mpShader->SetMatrix("LightViewMatrix", &matLightView);
	mpShader->SetMatrix("LightProjectionMatrix", &matLightProjection);*/
	mpShader->SetVector("WorldLightPosition", &GENERIC::worldLightPosition);
	mpShader->SetVector("WorldCameraPosition", &GENERIC::worldCameraPosition);
	//mpShader->SetVector("LightColor", &Vector4(0.7f, 0.7f, 0.7f, 1));
	mpShader->SetTexture("DiffuseMap_Tex", mpDiffuseTexture);
	//mpShader->SetTexture("SpecularMap_Tex", mpSpecularTexture);
	//mpShader->SetTexture("ShadowMap_Tex", mpShadowRenderTarget);

	
	UINT numPasses = 0;
	mpShader->Begin(&numPasses, 0);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			mpShader->BeginPass(i);
			{
				mpModel->DrawSubset(0);

				typedef std::vector<Component::Render*>::iterator listiterator;
				listiterator iter = listRenders.begin();
				while (iter != listRenders.end())
				{
					Component::Render* elemental = (*iter);
					elemental->DrawOnScreen(mpShader);
					++iter;
				}
			}
			mpShader->EndPass();
		}
	}
	mpShader->End();

	//포스트이펙트 처리 부분

	directDevice->SetRenderTarget(0, pHWBackBuffer);
	pHWBackBuffer->Release();
	pHWBackBuffer = nullptr;
	
	LPD3DXEFFECT effectToUse = mpGrayScale;
	effectToUse->SetTexture("Scene_Tex", mpScreenRenderTarget);
	effectToUse->Begin(&numPasses, 0);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{ 
			effectToUse->BeginPass(i);
			{
				directDevice->SetStreamSource(0, mpFullScreenQuadVB, 0, sizeof(float) * 5);
				directDevice->SetIndices(mpFullScreenQuadIB);
				directDevice->SetVertexDeclaration(mpFullScreenQuadDecl);

				directDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

			}
			effectToUse->EndPass();
		}
	}
	effectToUse->End();
	
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

const LPDIRECT3DDEVICE9 RenderDevice::GetDevice()
{
	return directDevice;
}