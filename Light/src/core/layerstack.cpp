#include "core/layerstack.hpp"

namespace Light
{
	LayerStack::LayerStack() 
	{
		
	}
	
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
		layers.push_back(layer);
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
		layers.insert(layers.begin() + layerTop, overlay);
		
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