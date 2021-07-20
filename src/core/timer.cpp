#include "pch.h"
#include "timer.h"

namespace engine
{
	Timer Timer::ms_timer;

	void Timer::init()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);

		m_freq = (double)freq.QuadPart;

		QueryPerformanceCounter(&m_timeTime);

		reset();
	}

	void Timer::reset()
	{
		QueryPerformanceCounter(&m_startTime);
	}

	void Timer::update()
	{
		QueryPerformanceCounter(&m_endTime);
		m_deltaTime = (m_endTime.QuadPart - m_startTime.QuadPart) / m_freq;
	}

	float Timer::getDelta()
	{
		return m_deltaTime;
	}

	float Timer::getTime()
	{
		return float(m_endTime.QuadPart - m_timeTime.QuadPart) / m_freq;
	}
}
