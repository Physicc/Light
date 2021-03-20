#ifndef __KEYEVENT_H__
#define __KEYEVENT_H__

#include "events/event.hpp"

namespace Light
{
	class KeyEvent : public Event
	{
	public:
		inline int getKeycode() const { return keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode): keycode(keycode) {}
		int keycode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatcount): KeyEvent(keycode), repeatcount(repeatcount) {}

		inline int getRepeatcount() { return repeatcount; }

		std::string ToString() const override
		{
			return "KeyPressedEvent: " + std::to_string(KeyEvent::keycode) + "(" + std::to_string(repeatcount) + " repeats)";

		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int repeatcount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode): KeyEvent(keycode) {}

		std::string ToString() const override
		{
			return "KeyReleasedEvent: " + std::to_string(keycode);

		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
#endif // __KEYEVENT_H__