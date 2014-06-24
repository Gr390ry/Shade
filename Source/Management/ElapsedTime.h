#pragma once

class ElapsedTime
{
public:
	static void Initialize()
	{
	}
	static double GetElapsedTime()
	{
		return 0;
	}
	static void UpdateTickCount()
	{
		LARGE_INTEGER qwTicksPerSec, qwFrequency;

		if (QueryPerformanceFrequency(&qwFrequency))
		{
		}

		currentQuery = 0;
	}

public:
	static double lastedQuery;
	static double currentQuery;
};

double ElapsedTime::currentQuery = 0;
double ElapsedTime::lastedQuery = 0;
