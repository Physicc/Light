#ifndef __LAYER_H__
#define __LAYER_H__

#include "core/base.hpp"
#include "events/event.hpp"
#include "core/timestep.hpp"

namespace Light
{
	class Layer
	{
	public:
		Layer(std::string name): m_name(name) {};
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onEvent(Event& e) {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImguiRender() {}

		inline const std::string& getName() { return m_name; }
		inline void blockHoverEvents(bool block) { m_hoverEventsBlocking = block; }
		inline void blockFocusEvents(bool block) { m_focusEventsBlocking = block; }
	protected:
		std::string m_name;

		bool m_hoverEventsBlocking = true;
		bool m_focusEventsBlocking = true;
	};
}

#endif // __LAYER_H__