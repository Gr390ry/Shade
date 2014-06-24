#pragma once
#include "../ISingleton.h"

class ElapsedTime : public ISingleton<ElapsedTime>
{
public:
	void Initialize()
	{
		LARGE_INTEGER qwFrequency, qwCounter;

		if (QueryPerformanceFrequency(&qwFrequency))
		{
			dbFrequency = static_cast<double>(qwFrequency.QuadPart);
		}

		QueryPerformanceCounter(&qwCounter);
		lastedQuery = static_cast<double>(qwCounter.QuadPart) / dbFrequency;		
	}
	double GetElapsedTime()
	{
		return elapsedQuery;
	}
	void UpdateTickCount()
	{
		LARGE_INTEGER qwCount;
		
		QueryPerformanceCounter(&qwCount);

		double QueryCounterVarable = static_cast<double>(qwCount.QuadPart);

		currentQuery = QueryCounterVarable / dbFrequency;
		elapsedQuery = currentQuery - lastedQuery;
		lastedQuery = currentQuery;

		char szMsg[256];
		sprintf_s(szMsg, 256, "Counter:%f\n", elapsedQuery);
		OutputDebugString(szMsg);
	}

private:
	double lastedQuery;
	double currentQuery;
	double elapsedQuery;
	double dbFrequency;
};
