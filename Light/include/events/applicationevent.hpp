#ifndef __APPLICATIONEVENT_H__
#define __APPLICATIONEVENT_H__

#include "events/event.hpp"

namespace Light
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int x, int y): x(x), y(y) {}

		std::tuple<int, int> getSize() { return std::make_tuple(x, y); }

		std::string ToString() const override
		{
			return "WindowResizeEvent: " + std::to_string(x) + ", " + std::to_string(y) + ")";
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int x,y;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};
}
#endif // __APPLICATIONEVENT_H__