#ifndef __INPUT_H__
#define __INPUT_H__

#include "core/base.hpp"

namespace Light {
	
	class Input
	{
	public:
		static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImpl(keycode); }
		static bool isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressedImpl(button); }
		static std::tuple<float, float> getMousePos() { return s_instance->getMousePosImpl(); }
	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::tuple<float, float> getMousePosImpl() = 0;
	private:
		static Input* s_instance;
	};

}

#endif // __INPUT_H__

