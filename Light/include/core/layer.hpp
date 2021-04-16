#ifndef __LAYER_H__
#define __LAYER_H__

#include "base.hpp"
#include "events/event.hpp"
#include "core/timestep.hpp"

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
		virtual void onUpdate(Timestep ts) {}
		virtual void onImguiRender() {}

		inline const std::string& getName() { return name; }
		inline void blockHoverEvents(bool block) { hoverEventsBlocking = block; }
		inline void blockFocusEvents(bool block) { focusEventsBlocking = block; }
	protected:
		std::string name;

		bool hoverEventsBlocking = true;
		bool focusEventsBlocking = true;
	};
}

#endif // __LAYER_H__