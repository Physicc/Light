#ifndef __OPENGLCONTEXT_H__
#define __OPENGLCONTEXT_H__

#include "light/rendering/graphicscontext.hpp"
#include "core/logging.hpp"
#include "glad/gl.h"

#include <utility>

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

		inline auto getGLVersion() const { return std::pair<int, int>(m_majorVersion, m_minorVersion); }
	private:
		GLFWwindow* m_windowHandle;
		GLint m_majorVersion = 0;
		GLint m_minorVersion = 0;
	};
}

#endif // __OPENGLCONTEXT_H__
