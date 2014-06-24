#include "ShadeEngine.h"
#include "RenderDevice.h"
#include "Management\ElapsedTime.h"
#include "General.h"

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessInput(HWND, WPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		GENERIC::gAppName.c_str(), NULL };
	RegisterClassEx(&wc);

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindow(GENERIC::gAppName.c_str(), GENERIC::gAppName.c_str(),
		style, CW_USEDEFAULT, 0, GENERIC::windowWidth, GENERIC::windowHeight,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	if (!RenderDevice::Get()->InitializeDevice(hWnd))
	{
		MessageBox(hWnd, "Render Device Initialize Failure!", "Error", MB_OK);
		PostQuitMessage(0);
	}
	if (!General::Get()->InitializeGame())
	{
		MessageBox(hWnd, "General Game Initialize Failure!", "Error", MB_OK);
		PostQuitMessage(0);
	}

	RenderDevice::Get()->LoadAsset();

	// Client Rect 크기가 WIN_WIDTH, WIN_HEIGHT와 같도록 크기를 조정한다.
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

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);	

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	ElapsedTime::Initialize();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ElapsedTime::UpdateTickCount();

		General::Get()->Update(0);
		RenderDevice::Get()->RenderFrame();		
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
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
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