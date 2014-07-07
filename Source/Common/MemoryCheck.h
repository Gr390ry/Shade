#pragma once
#include "ShadeEngine.h"

class MemoryCheck
{
public:
	bool operator () ()
	{
		//System Memory Check
		MEMORYSTATUS MemStatus;
		MemStatus.dwLength = sizeof(MemStatus);

		GlobalMemoryStatus(&MemStatus);

		//Video Memory Check
		LPDIRECTDRAW7 lpDD = NULL;
		DDSCAPS2      ddsCaps2;
		DWORD         dwTotal;
		DWORD         dwFree;

		if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL)))
		{
			return false;
		}

		// Initialize the structure.
		ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
		ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;

		if (FAILED(lpDD->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree)))
		{
			if (lpDD) { lpDD->Release(); }
			return false;
		}

		if (lpDD) { lpDD->Release(); }
		return true;
	}
};