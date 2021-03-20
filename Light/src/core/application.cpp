#include "core/application.hpp"

namespace Light
{

	Application::Application()
	{
		window = (std::unique_ptr<Window>)Window::create();
		window->setEventCallback(BIND_EVENT_FN(onEvent));
	}
	
	Application::~Application() {}

	void Application::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

		std::clog << e << std::endl;

		for(auto it = layerstack.end(); it != layerstack.begin(); it--)
		{
			(*it)->onEvent(e);
			if(e.handled)
				break;
		}
	}

	void Application::run() 
	{
		while(running)
		{

			for(Layer* layer : layerstack)
			{
				layer->onUpdate();
			}

			window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	void Application::pushLayer(Layer* layer)
	{
		layerstack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay)
	{
		layerstack.pushOverlay(overlay);
	}
}