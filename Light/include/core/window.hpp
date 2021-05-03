#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "core/base.hpp"
#include "events/event.hpp"

namespace Light
{
	struct WindowProps
	{
		std::string title;
		int width;
		int height;

		WindowProps(const std::string title = "Light Engine", int width = 1600, int height = 900) : width(width), height(height), title(title) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}

#endif // __WINDOW_H__