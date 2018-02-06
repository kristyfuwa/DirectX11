#pragma once
#include <windows.h>
class Timer
{
public:
	Timer();
	Timer(const Timer& other);
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64		m_iFrequency;
	float		m_fTickPerMs;
	INT64		m_iStartTime;
	float       m_fFrameTime;
};

