#include "platform/glfw/windowglfw.hpp"
#include "platform/opengl/openglcontext.hpp"
#include "events/keyevent.hpp"
#include "events/mouseevent.hpp"
#include "events/applicationevent.hpp"

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

	static void GLFWErrorCallback(int error, const char* description)
	{
		LIGHT_CORE_ERROR("GLFW Error({}): {}", error, description);
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

		#if __APPLE__
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		if(!m_window)
		{
			LIGHT_CORE_CRITICAL("Could not create window \'{2}\' of size {0}x{1}", props.width, props.height, props.title);
		}
		LIGHT_CORE_INFO("Created window \'{2}\' of size {0}x{1}", props.width, props.height, props.title);
		m_context = new OpenGLContext(m_window);
		m_context->init();

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
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