#pragma once
#include "ISingleton.h"

class GameTimer : public ISingleton<GameTimer>
{
public:
	GameTimer();

	void Initialize();
	void UpdateTickCount();

	void Reset();
	void Start();
	void Stop();

	float DeltaTime() const;
	float TotalTime() const;

private:
	
	double mDeltaTime;
	double mSecPerCount;
	__int64 mBaseTime;
	__int64 mPauseTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrentTime;

	bool mIsStopped;
};
