#ifndef __WINDOWGLFW_H__
#define __WINDOWGLFW_H__


#include "core/base.hpp"
#include "core/window.hpp"
#include "light/rendering/graphicscontext.hpp"

#include "GLFW/glfw3.h"

namespace Light
{

	class WindowGlfw : public Window
	{
	public:
		WindowGlfw(const WindowProps& props);

		virtual ~WindowGlfw();

		void onUpdate() override;

		virtual uint32_t getWidth() const override { return m_data.width;};
		virtual uint32_t getHeight() const override { return m_data.height; };

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) override { m_data.callback = callback; };
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		virtual void* getNativeWindow() const override { return m_window; };

	private:
		void init(const WindowProps& props);
		void shutdown();

		bool tryCreateContext(int versionMajor, int versionMinor);
		void createBestContext();

		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string title;
			int width, height;
			bool vSync;

			EventCallbackFn callback;
		};

		WindowData m_data;
	};
}

#endif // __WINDOWGLFW_H__
