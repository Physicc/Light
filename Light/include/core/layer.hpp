#ifndef __LAYER_H__
#define __LAYER_H__

#include "base.hpp"
#include "events/event.hpp"

namespace Light
{
	class Layer
	{
	public:
		Layer(std::string name): name(name) {};
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onEvent(Event& e) {}
		virtual void onUpdate() {}
		virtual void onImguiRender() {}

		inline const std::string& getName() { return name; }
	protected:
		std::string name;
	};
}

#endif // __LAYER_H__