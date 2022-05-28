#include "core/logging.hpp"
// Include logging.hpp BEFORE windowglfw.hpp because windowglfw.hpp
// includes Windows.h and windowglfw.hpp includes glad.h which
// should be included after Windows.h (APIENTRY Macro redefinition warning)
#include "platform/glfw/windowglfw.hpp"
#include "light/platform/opengl/openglcontext.hpp"
#include "events/keyevent.hpp"
#include "events/mouseevent.hpp"
#include "events/applicationevent.hpp"
#include "core/base.hpp"

#include "stb_image.h"

namespace Light
{
	static bool glfwInitialized = false;

	Window* Window::create(const WindowProps& props)
	{
		return new WindowGlfw(props);
	}

	WindowGlfw::WindowGlfw(const WindowProps& props)
	{
		init(props);
	}

	WindowGlfw::~WindowGlfw()
	{
		shutdown();
	}

	static void GLFWErrorCallback([[maybe_unused]] int error, [[maybe_unused]] const char* description)
	{
		if(error != GLFW_VERSION_UNAVAILABLE)
		{
			LIGHT_CORE_ERROR("GLFW Error({}): {}", error, description);
		}
		else
		{
			LIGHT_CORE_WARN("GLFW Error({}): {}", error, description);
		}
	}

	bool WindowGlfw::tryCreateContext(int versionMajor, int versionMinor)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#if __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		#endif
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

		// Check if context was created
		if (m_window == nullptr)
		{
			LIGHT_CORE_DEBUG("Could not create opengl context with version {}.{}", versionMajor, versionMinor);
			return false;
		}

		return true;
	}

	void WindowGlfw::createBestContext()
	{
		// List of opengl versions to try
		const std::pair<int, int> versions[] = {
			{ 4, 6},
			{ 4, 5},
			{ 4, 4},
			{ 4, 3},
			{ 4, 2},
			{ 4, 1},
			{ 4, 0},
			{ 3, 3},
		};

		// Try to create context with each version
		for (const auto& version : versions)
		{
			if (tryCreateContext(version.first, version.second))
			{
				LIGHT_CORE_DEBUG("Created opengl context with version {}.{}", version.first, version.second);
				glfwShowWindow(m_window);
				return;
			}
		}
	}

	void WindowGlfw::init(const WindowProps& props)
	{
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

		if(!glfwInitialized)
		{
			int success = glfwInit();
			if(!success)
			{
				LIGHT_CORE_CRITICAL("Could not initialize GLFW");
				exit(1);
			}

			glfwSetErrorCallback(GLFWErrorCallback);
			glfwInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		createBestContext();

		if(!m_window)
		{
			LIGHT_CORE_CRITICAL("Could not create window \'{2}\' of size {0}x{1}", props.width, props.height, props.title);
			exit(1);
		}
		LIGHT_CORE_INFO("Created window \'{2}\' of size {0}x{1}", props.width, props.height, props.title);
		m_context = new OpenGLContext(m_window);
		m_context->init();


		GLFWimage icon;
		icon.pixels = stbi_load("assets/icon/light.png", &icon.width, &icon.height, nullptr, 4);
		glfwSetWindowIcon(m_window, 1, &icon);


		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data->callback(event);
					break;
				}
			case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data->callback(event);
					break;
				}
			case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data->callback(event);
					break;
				}
			default:
				break;
			}

		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			if(action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(button);
				data->callback(event);
			} else
			if(action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event(button);
				data->callback(event);
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xoffset, yoffset);
			data->callback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xpos, ypos);
			data->callback(event);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			data->width = width;
			data->height = height;

			WindowResizeEvent event(width, height);
			data->callback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data->callback(event);
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codepoint)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(codepoint);
			data->callback(event);
		});

		glfwSetWindowUserPointer(m_window, &m_data);

		setVSync(true);
	}

	void WindowGlfw::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowGlfw::onUpdate()
	{
		glfwPollEvents();
		m_context->swapBuffers();
	}

	void WindowGlfw::setVSync(bool enabled)
	{
		if(enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

        m_data.vSync = enabled;
	}

	bool WindowGlfw::isVSync() const
	{
		return m_data.vSync;
	}
}
