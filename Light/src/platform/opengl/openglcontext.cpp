#include "platform/opengl/openglcontext.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Light
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle)
	{
		if(!windowHandle)
		{
			std::cerr << "Handle is NULL!" << std::endl;
			exit(1);
		}
	}
	
	OpenGLContext::~OpenGLContext() = default;
	
	void OpenGLContext::init() 
	{
		glfwMakeContextCurrent(m_windowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if(!success)
			exit(1);
	}
	
	void OpenGLContext::swapBuffers() 
	{
		glfwSwapBuffers(m_windowHandle);
	}
}