#include "core/layerstack.hpp"

namespace Light
{
	LayerStack::LayerStack() = default;
	
	LayerStack::~LayerStack() 
	{
		for(Layer* layer : layers)
		{
			layer->onDetach();
			delete layer;
		}
	}
	
	void LayerStack::pushLayer(Layer* layer) 
	{
		layers.insert(layers.begin() + layerTop, layer);
		layerTop++;
		
	}
	
	void LayerStack::popLayer(Layer* layer) 
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if(it != layers.end())
		{
			(*it)->onDetach();
			layers.erase(it);
			layerTop--;
		}
	}

	void LayerStack::pushOverlay(Layer* overlay) 
	{
		layers.push_back(overlay);
	}
	
	void LayerStack::popOverlay(Layer* overlay) 
	{
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if(it != layers.end())
		{
			(*it)->onDetach();
			layers.erase(it);
		}
	}
	
}