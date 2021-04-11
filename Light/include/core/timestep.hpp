#ifndef __TIMESTEP_H__
#define __TIMESTEP_H__

#include "core/base.hpp"

namespace Light
{
	class Timestep
	{
	public:
		Timestep(float time) : time(time) {}

		float getSeconds() { return time; }
		float getMilliSeconds() { return time * 1000.0f; }
	private:
		float time;
	};

}

#endif // __TIMESTEP_H__