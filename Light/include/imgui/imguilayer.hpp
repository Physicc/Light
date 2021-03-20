#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "core/layer.hpp"
#include "platform/opengl/imgui_impl_opengl3.h"
#include "events/applicationevent.hpp"
#include "events/mouseevent.hpp"
#include "events/keyevent.hpp"

namespace Light
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer(std::string name) : Layer(name) {}
		~ImguiLayer();

		void onAttach();
		void onDetach();
		void onEvent(Event& e);
		void onUpdate();
	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);

		float time = 0.0;
	};
}


#endif // __IMGUILAYER_H__