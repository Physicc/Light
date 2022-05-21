#include "light/platform/opengl/openglcontext.hpp"

#include "GLFW/glfw3.h"

#include <string>

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

#ifndef NDEBUG
	static void glDebugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message,
		[[maybe_unused]] const void* userParam)
	{
		auto const src_str = [&]() -> std::string
		{
			switch(source)
			{
				case GL_DEBUG_SOURCE_API: return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
				case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
				case GL_DEBUG_SOURCE_APPLICATION: return "Application";
				case GL_DEBUG_SOURCE_OTHER: return "Other";
				default: return "Unknown";
			}
		}();

		auto const type_str = [&]() -> std::string
		{
			switch(type)
			{
				case GL_DEBUG_TYPE_ERROR: return "Error";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
				case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
				case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
				case GL_DEBUG_TYPE_MARKER: return "Marker";
				case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
				case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
				case GL_DEBUG_TYPE_OTHER: return "Other";
				default: return "Unknown";
			}
		}();

		auto const severity_str = [&]() -> std::string
		{
			switch(severity)
			{
				case GL_DEBUG_SEVERITY_HIGH: return "High";
				case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
				case GL_DEBUG_SEVERITY_LOW: return "Low";
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
				default: return "Unknown";
			}
		}();

		std::string msg;
		msg.reserve(length);
		msg.assign(message, length);

		if(severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			LIGHT_CORE_INFO("OpenGL({0}, {1}, {2}, {3}): {4}", id, src_str, type_str, severity_str, msg);
		}
		else if(severity == GL_DEBUG_SEVERITY_HIGH)
		{
			LIGHT_CORE_ERROR("OpenGL({0}, {1}, {2}, {3}): {4}", id, src_str, type_str, severity_str, msg);
		}
		else if(severity == GL_DEBUG_SEVERITY_MEDIUM)
		{
			LIGHT_CORE_WARN("OpenGL({0}, {1}, {2}, {3}): {4}", id, src_str, type_str, severity_str, msg);
		}
		else if(severity == GL_DEBUG_SEVERITY_LOW)
		{
			LIGHT_CORE_DEBUG("OpenGL({0}, {1}, {2}, {3}): {4}", id, src_str, type_str, severity_str, msg);
		}
	}
#endif

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int version = gladLoadGL(glfwGetProcAddress);
		if(!version)
		{
			LIGHT_CORE_CRITICAL("Could not initialize GLAD");
			exit(1);
		}

#ifndef NDEBUG
		if(GLAD_GL_VERSION_4_3 || GLAD_GL_KHR_debug)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
		}
		else if (GLAD_GL_ARB_debug_output)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
			glDebugMessageCallbackARB(glDebugCallback, nullptr);
			glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, nullptr, GL_FALSE);
		}
#endif

		LIGHT_CORE_INFO("OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		LIGHT_CORE_INFO("OpenGL Device: {0}", glGetString(GL_RENDERER));
		LIGHT_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}
