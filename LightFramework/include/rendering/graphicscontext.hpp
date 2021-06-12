#ifndef __GRAPHICSCONTEXT_H__
#define __GRAPHICSCONTEXT_H__

#include "core/base.hpp"

namespace Light
{
	class GraphicsContext
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};
}

#endif // __GRAPHICSCONTEXT_H__