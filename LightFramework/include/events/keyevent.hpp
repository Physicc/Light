#ifndef __KEYEVENT_H__
#define __KEYEVENT_H__

#include "events/event.hpp"

namespace Light
{
	class KeyEvent : public Event
	{
	public:
		inline int getKeycode() const { return m_keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode): m_keycode(keycode) {}
		int m_keycode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatcount): KeyEvent(keycode), m_repeatCount(repeatcount) {}

		inline int getRepeatCount() { return m_repeatCount; }

		std::string ToString() const override
		{
			return "KeyPressedEvent: " + std::to_string(KeyEvent::m_keycode) + "(" + std::to_string(m_repeatCount) + " repeats)";

		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode): KeyEvent(keycode) {}

		std::string ToString() const override
		{
			return "KeyReleasedEvent: " + std::to_string(m_keycode);

		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode): KeyEvent(keycode) {}

		std::string ToString() const override
		{
			return "KeyTypedEvent: " + std::to_string(m_keycode);

		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
#endif // __KEYEVENT_H__

