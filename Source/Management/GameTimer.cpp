#include "GameTimer.h"
#include <Windows.h>

GameTimer::GameTimer() : mDeltaTime(0), mSecPerCount(0), mBaseTime(0), mPauseTime(0), mStopTime(0), mPrevTime(0), mCurrentTime(0), mIsStopped(false)
{
	
}

//성능 카운터의 주파수를 조회해서 개수당 초 수를 설정한다.
void GameTimer::Initialize()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecPerCount = 1.f / (double)countsPerSec;

	Reset();
}

void GameTimer::Reset()
{
	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	mBaseTime = currentTime;
	mPrevTime = currentTime;
	mStopTime = 0;
	mIsStopped = false;
}

void GameTimer::Stop()
{
	if (mIsStopped)
	{
		return;
	}

	__int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	mStopTime	= currentTime;
	mIsStopped	= true;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mIsStopped)
	{
		mPauseTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mIsStopped = false;
	}
}

void GameTimer::UpdateTickCount()
{
	if (mIsStopped)
	{
		mDeltaTime = 0;
		return;
	}

	//현 프레임의 시간을 얻는다.
	{
		__int64 currentTime;

		QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		mCurrentTime = currentTime;

		mDeltaTime = (mCurrentTime - mPrevTime) * mSecPerCount;

		if (mDeltaTime < 0)
		{
			mDeltaTime = 0;
		}

		mPrevTime = mCurrentTime;
	}
}

float GameTimer::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

float GameTimer::TotalTime() const
{
	if (mIsStopped)
	{
		return (float)(((mStopTime - mPauseTime) - mBaseTime) * mSecPerCount);
	}
	return (float)(((mCurrentTime - mPauseTime) - mBaseTime) * mSecPerCount);
}