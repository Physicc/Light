#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "core/layer.hpp"
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

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImguiRender() override;

		void begin();
		void end();
	};
}


#endif // __IMGUILAYER_H__