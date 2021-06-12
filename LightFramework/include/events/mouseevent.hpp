#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__

#include "events/event.hpp"

namespace Light
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y): m_x(x), m_y(y) {}

		inline std::tuple<double, double> getPos() { return {m_x, m_y}; }

		std::string ToString() const override
		{
			return "MouseMovedEvent: (" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		double m_x, m_y;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double x, double y): x(x), y(y) {}

		inline std::tuple<double, double> getOffset() { return {x,y}; }

		std::string ToString() const override
		{
			return "MouseScrolledEvent: (" + std::to_string(x) + "," + std::to_string(y) + ")";
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		double x,y;
	};

	class MouseButtonEvent : public Event
	{
	public:
		int getButton() { return button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button): button(button) {}

		int button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button): MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			return "MouseButtonPressedEvent: " + std::to_string(button);
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			return "MouseButtonReleasedEvent: " + std::to_string(button);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
#endif // __MOUSEEVENT_H__