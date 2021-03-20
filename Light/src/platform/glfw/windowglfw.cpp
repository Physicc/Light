#include "platform/glfw/windowglfw.hpp"
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
		std::cerr << "GLFW Error(" << error << "): " << description << std::endl;
	}

	void WindowGlfw::init(const WindowProps& props)
	{
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		if(!glfwInitialized)
		{
			int success = glfwInit();
			if(!success)
				exit(1);

			glfwSetErrorCallback(GLFWErrorCallback);
			glfwInitialized = true;
		}

		window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
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

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event(xoffset, yoffset);
			data->callback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event(xpos, ypos);
			data->callback(event);
		});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);
			data->callback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
		{
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data->callback(event);
		});

		glfwSetWindowUserPointer(window, &data);

		setVSync(true);
	}

	void WindowGlfw::shutdown()
	{
		glfwDestroyWindow(window);
	}

	void WindowGlfw::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowGlfw::setVSync(bool enabled)
	{
		if(enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		
		data.vSync = enabled;
	}

	bool WindowGlfw::isVSync() const
	{
		return data.vSync;
	}
}