#ifndef __APPLICATIONEVENT_H__
#define __APPLICATIONEVENT_H__

#include "events/event.hpp"

namespace Light
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int x, int y): m_x(x), m_y(y) {}

		std::tuple<int, int> getSize() { return {m_x, m_y}; }

		[[nodiscard]] std::string ToString() const override
		{
			return "WindowResizeEvent: " + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int m_x,m_y;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		[[nodiscard]] std::string ToString() const override
		{
			return "WindowCloseEvent";
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};
}
#endif // __APPLICATIONEVENT_H__