#ifndef __LAYERSTACK_H__
#define __LAYERSTACK_H__

#include "core/base.hpp"
#include "core/layer.hpp"

namespace Light
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		int m_layerTop = 0;
	};
	
}

#endif // __LAYERSTACK_H__

