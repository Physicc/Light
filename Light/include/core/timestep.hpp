#ifndef __TIMESTEP_H__
#define __TIMESTEP_H__

#include "core/base.hpp"

namespace Light
{
	class Timestep
	{
	public:
		Timestep(float time) : m_time(time) {}

		float getSeconds() { return m_time; }
		float getMilliSeconds() { return m_time * 1000.0f; }
	private:
		float m_time;
	};

}

#endif // __TIMESTEP_H__