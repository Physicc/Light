#include "core/application.hpp"

#include "glad/glad.h"

namespace Light
{
	Application* Application::instance = nullptr;

	Application::Application()
	{
		if(instance == nullptr)
			instance = this;
		else
		{
			std::cerr << "Application already created" << std::endl;
			exit(1);
		}
		window = (std::unique_ptr<Window>)Window::create();
		window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		imguilayer = new ImguiLayer("ImGui Layer");
		pushOverlay(imguilayer);

	}
	
	Application::~Application() {}

	void Application::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

		std::clog << e << std::endl;

		for(auto it = layerstack.end(); it != layerstack.begin();)
		{
			(*--it)->onEvent(e);
			if(e.handled)
				break;
		}
	}

	void Application::run() 
	{
		while(running)
		{
			glClearColor(0.2,0.2,0.2,1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			for(Layer* layer : layerstack)
			{
				layer->onUpdate();
			}

			imguilayer->begin();
			for(Layer* layer : layerstack)
			{
				layer->onImguiRender();
			}
			imguilayer->end();

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
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay)
	{
		layerstack.pushOverlay(overlay);
		overlay->onAttach();
	}
}