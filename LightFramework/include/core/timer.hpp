#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include "core/logging.hpp"

namespace Light
{

	enum class TimerPrecision
	{
		Milliseconds,
		Microseconds,
		Nanoseconds
	};

	class Timer
	{
	public:
		Timer() { start(); }
		~Timer() = default;

		inline void start() { m_started = true; m_startTime = std::chrono::high_resolution_clock::now(); }
		inline void stop() { m_started = false; }

		/*
		 * @brief Returns the time since start() called.
		 * If timer is stopped, then returns 0.0
		 *
		 * @tparam precision The precision of the returned value. Has to be std::chrono::precision.
		 * @return Time since start() called
		 */
		template<class precision = std::chrono::milliseconds>
		double getDeltaTime()
		{
			if (m_started)
			{
				auto now = std::chrono::high_resolution_clock::now();
				auto delta = now - m_startTime;
				return std::chrono::duration<double, typename precision::period>(delta).count();
			}
			else
			{
				return 0.0;
			}
		}

		inline bool isStarted() { return m_started; }

	private:
		std::chrono::high_resolution_clock::time_point m_startTime;
		std::chrono::high_resolution_clock::time_point m_pausedTime;

		bool m_started;
	};
} // namespace Light


#endif // __TIMER_H__
