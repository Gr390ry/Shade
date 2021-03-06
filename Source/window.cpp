#include "ShadeEngine.h"
#include "resource.h"
#include "Management/General.h"
#include "Management/RenderDevice.h"
#include "Management/GameTimer.h"

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessInput(HWND, WPARAM);
void CalculateFrameStats(HWND);
void SetWindowRect(HWND);
void InitializeObjects(HWND);

bool gAppPuased;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
#if defined (DEBUG) | defined (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//아이콘 로드
	HICON hIcon = reinterpret_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2),
											IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR));
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
					GetModuleHandle(NULL),
					hIcon,
					NULL, 
					NULL, 
					NULL,
					GENERIC::gAppName.c_str(),
					hIcon };
	RegisterClassEx(&wc);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(GENERIC::gAppName.c_str(), GENERIC::gAppName.c_str(),
		style, CW_USEDEFAULT, 0, GENERIC::windowWidth, GENERIC::windowHeight,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);


	Console::Get()->Initialize("Debug Console", hInstance, hPrevInstance, lpszCmdParam, nCmdShow);
	DEBUG_CONSOLE("Hello!\n");	

	gAppPuased = false;
	InitializeObjects(hWnd);
	SetWindowRect(hWnd);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);	

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GameTimer::Get()->UpdateTickCount();


		if (!gAppPuased)
		{
			float tick = GameTimer::Get()->DeltaTime();

			General::Get()->Update(tick);
			//RenderDevice::Get()->UpdateScene(tick);
			RenderDevice::Get()->Render11();
			CalculateFrameStats(hWnd);
		}
		else
		{
			Sleep(1);
		}		
	}

	RenderDevice::Get()->Release();
	UnregisterClass(GENERIC::gAppName.c_str(), wc.hInstance);
	return 0;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		ProcessInput(hWnd, wParam);
		break;
	case WM_ACTIVATE:	//응용 프로그램이 활성화 되어 있지 않을때 타이머를 정지 시킨다.
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			gAppPuased = true;
			GameTimer::Get()->Stop();
		}
		else
		{
			gAppPuased = false;
			GameTimer::Get()->Start();
		}
		break;
	case WM_EXITSIZEMOVE:
		break;
	case WM_MENUCHAR: //메뉴가 활성화 되어서 사용자가 키를 눌렀으나 그 키가 그 어떤 니모닉이나 단축키에도 해당하지 않을때 전달
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void SetWindowRect(HWND hWnd)
{
	POINT ptDiff;
	RECT rcClient, rcWindow;

	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;

	MoveWindow(hWnd, rcWindow.left, rcWindow.top,
		GENERIC::windowWidth + ptDiff.x,
		GENERIC::windowHeight + ptDiff.y, TRUE);
}

void InitializeObjects(HWND hWnd)
{
	if (!RenderDevice::Get()->InitializeDevice11(hWnd))
	{
		MessageBox(hWnd, "Render Device Initialize Failure!", "Error", MB_OK);
		PostQuitMessage(0);
	}
	Console::Get()->print("DirectX11 Device Initialize Complete!\n");

	if (!General::Get()->LoadAssets())
	{
		MessageBox(hWnd, "General Load Assets Failure!", "Error", MB_OK);
		PostQuitMessage(0);
	}

	if (!General::Get()->InitializeGame())
	{
		MessageBox(hWnd, "General Game Initialize Failure!", "Error", MB_OK);
		PostQuitMessage(0);
	}
	GameTimer::Get()->Initialize();

	//TODO:인스탄싱 테스트
	RenderDevice::Get()->BuildInstancedBuffer();

	Console::Get()->print("Game Asset Initialize Complete!\n");
}

void ProcessInput(HWND hWnd, WPARAM keyPress)
{
	switch (keyPress)
	{
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		break;
	}
}

void CalculateFrameStats(HWND hWnd)
{
	static int			frameCount = 0;
	static float		timeElapsed = 0;;

	++frameCount;

	float totalTime = GameTimer::Get()->TotalTime();
	if ((totalTime - timeElapsed) >= 1.f)
	{
		float fps = (float)frameCount;
		float mspf = 1000.f / fps;
		char caption[256] = {0,};

		sprintf_s(caption, "%s FPS:%.2f FrameTime:%.2f", GENERIC::gAppName.c_str(), fps, mspf);

		SetWindowText(hWnd, caption);

		frameCount = 0;
		timeElapsed += 1;
	}
}