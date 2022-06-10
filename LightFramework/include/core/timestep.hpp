#ifndef __TIMESTEP_H__
#define __TIMESTEP_H__

#include "core/base.hpp"

namespace Light
{
	class Timestep
	{
	public:
		/*
		 * @brief Constructor
		 * @param time Time in seconds
		 */
		Timestep(double time) : m_time(time) {}

		double getSeconds() { return m_time; }
		double getMilliSeconds() { return m_time * 1000.0f; }
	private:
		double m_time;
	};

}

#endif // __TIMESTEP_H__
