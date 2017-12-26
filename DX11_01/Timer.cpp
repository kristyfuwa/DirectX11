#include "Timer.h"



Timer::Timer()
{

}


Timer::Timer(const Timer& other)
{

}

Timer::~Timer()
{
}

bool Timer::Initialize()
{
	// 检测系统是否支持高精度计时器.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_iFrequency);
	if (m_iFrequency == 0)
		return false;

	//// 得到每毫秒多少个计数器.
	m_fTickPerMs = (float)(m_iFrequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_iStartTime);

	return true;
}
//每个渲染帧都会被调用，从而计算出每帧之间的时间
void Timer::Frame()
{
	INT64 currentTime;
	float timeDifferenc;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDifferenc = (float)(currentTime - m_iStartTime);
	m_fFrameTime = timeDifferenc / m_fTickPerMs;
	m_iStartTime = currentTime;
}

float Timer::GetTime()
{
	return m_fFrameTime;
}
