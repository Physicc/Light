#include "light/platform/opengl/openglcontext.hpp"

#include "core/logging.hpp"

#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include "string"

namespace Light
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle)
	{
		if(!windowHandle)
		{
			LIGHT_CORE_CRITICAL("Could not create OpenGL Context: Handle is NULL");
			exit(1);
		}
	}

	OpenGLContext::~OpenGLContext() = default;

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int success = gladLoadGL(glfwGetProcAddress);
		if(!success)
		{
			LIGHT_CORE_CRITICAL("Could not initialize GLAD");
			exit(1);
		}

		LIGHT_CORE_INFO("OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		LIGHT_CORE_INFO("OpenGL Device: {0}", glGetString(GL_RENDERER));
		LIGHT_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}
