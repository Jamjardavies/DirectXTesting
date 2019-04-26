#include "Timer.h"

Timer::Timer()
{
	m_last = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const std::chrono::steady_clock::time_point old = m_last;

	m_last = std::chrono::steady_clock::now();

	const std::chrono::duration<float> frameTime = m_last - old;

	return frameTime.count();
}

float Timer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_last).count();
}
