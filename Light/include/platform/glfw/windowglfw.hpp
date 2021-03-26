#ifndef __WINDOWGLFW_H__
#define __WINDOWGLFW_H__


#include "core/base.hpp"
#include "core/window.hpp"
#include "rendering/graphicscontext.hpp"

#include "GLFW/glfw3.h"

namespace Light
{

	class WindowGlfw : public Window
	{
	public:
		WindowGlfw(const WindowProps& props);

		virtual ~WindowGlfw();

		void onUpdate() override;

		virtual uint32_t getWidth() const override { return data.width;};
		virtual uint32_t getHeight() const override { return data.height; };

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) override { data.callback = callback; };
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const override { return window; };
	
	private:
		void init(const WindowProps& props);
		void shutdown();

		GLFWwindow* window;
		GraphicsContext* context;
		
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn callback;
		};

		WindowData data;
	};
}

#endif // __WINDOWGLFW_H__