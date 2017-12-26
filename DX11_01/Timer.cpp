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
	// ���ϵͳ�Ƿ�֧�ָ߾��ȼ�ʱ��.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_iFrequency);
	if (m_iFrequency == 0)
		return false;

	//// �õ�ÿ������ٸ�������.
	m_fTickPerMs = (float)(m_iFrequency / 1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_iStartTime);

	return true;
}
//ÿ����Ⱦ֡���ᱻ���ã��Ӷ������ÿ֮֡���ʱ��
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
