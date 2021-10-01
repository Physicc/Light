#include "core/layerstack.hpp"

namespace Light
{
	LayerStack::LayerStack() = default;
	
	LayerStack::~LayerStack() 
	{
		for(Layer* layer : m_layers)
		{
			layer->onDetach();
			delete layer;
		}
	}
	
	void LayerStack::pushLayer(Layer* layer) 
	{
		m_layers.insert(m_layers.begin() + m_layerTop, layer);
		m_layerTop++;
		
	}
	
	void LayerStack::popLayer(Layer* layer) 
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if(it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
			m_layerTop--;
		}
	}

	void LayerStack::pushOverlay(Layer* overlay) 
	{
		m_layers.push_back(overlay);
	}
	
	void LayerStack::popOverlay(Layer* overlay) 
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if(it != m_layers.end())
		{
			(*it)->onDetach();
			m_layers.erase(it);
		}
	}
	
}

