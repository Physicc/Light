#include "platform/opengl/openglcontext.hpp"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Light
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : windowHandle(windowHandle)
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
		glfwMakeContextCurrent(windowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if(!success)
		{
			LIGHT_CORE_CRITICAL("Could not initialize GLAD");
			exit(1);
		}
	}
	
	void OpenGLContext::swapBuffers() 
	{
		glfwSwapBuffers(windowHandle);
	}
}