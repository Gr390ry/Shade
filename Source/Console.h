#pragma once
#include <Windows.h>
#include <iostream>
#include <memory>
#include "ISingleton.h"

class Console : public ISingleton<Console>
{
public:
	bool Initialize(const char * title, HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
#ifdef _DEBUG
#else
		return true;
#endif
		AllocConsole();
		SetConsoleTitleA(title);

		hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
		hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		hConsoleError = GetStdHandle(STD_ERROR_HANDLE);

		return true;
	}

	void print(const char* foramt, ...)
	{
#ifdef _DEBUG
#else
		return;
#endif
		char buff[1024];

		va_list va;

		va_start(va, foramt);
		vsprintf_s(buff, foramt, va);
		va_end(va);		

		DWORD numberOfCharsWritten = 0;

		BOOL bSuccess = WriteConsoleA(
			hConsoleOutput,
			buff,
			(DWORD)strlen(buff),
			&numberOfCharsWritten,
			nullptr);
		OutputDebugStringA(buff);
	}

private:
	HANDLE hConsoleInput;
	HANDLE hConsoleOutput;
	HANDLE hConsoleError;
};