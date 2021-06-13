#ifndef __INPUTGLFW_H__
#define __INPUTGLFW_H__

#include "core/input.hpp"

namespace Light
{
	class InputGlfw : public Input
	{
	protected:
		bool isKeyPressedImpl(int keycode) override;
		bool isMouseButtonPressedImpl(int button) override;
		std::tuple<float, float> getMousePosImpl() override;
	};

}

#endif // __INPUTGLFW_H__