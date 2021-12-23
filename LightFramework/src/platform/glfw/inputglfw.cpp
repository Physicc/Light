#include "platform/glfw/inputglfw.hpp"

#include "core/application.hpp"

#include "GLFW/glfw3.h"

namespace Light
{
	Input* Input::s_instance = new InputGlfw;

	bool InputGlfw::isKeyPressedImpl(int keycode) 
	{
		auto window = (GLFWwindow*)Application::get().getWindow().getNativeWindow();
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool InputGlfw::isMouseButtonPressedImpl(int button) 
	{
		auto window = (GLFWwindow*)Application::get().getWindow().getNativeWindow();
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::tuple<float, float> InputGlfw::getMousePosImpl() 
	{
		auto window = (GLFWwindow*)Application::get().getWindow().getNativeWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return {(float)xpos, (float)ypos};
	}
}
