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
		virtual void onEvent(Event&) {}
		virtual void onUpdate(Timestep) {}
		virtual void onImguiRender() {}

		inline const std::string& getName() { return m_name; }
	protected:
		std::string m_name;
	};
}

#endif // __LAYER_H__
