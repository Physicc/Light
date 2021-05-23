#ifndef __OPENGLCONTEXT_H__
#define __OPENGLCONTEXT_H__

#include "rendering/graphicscontext.hpp"

struct GLFWwindow;

namespace Light
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		void init() override;
		void swapBuffers() override;
	private:
		GLFWwindow* m_windowHandle;
	};	
}

#endif // __OPENGLCONTEXT_H__