#include "core/application.hpp"

#include "rendering/renderer.hpp"

//Temp for time
#include "GLFW/glfw3.h"

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
		window = static_cast<std::unique_ptr<Window>>(Window::create());
		window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::init();

		imguilayer = new ImguiLayer("ImGui Layer");
		pushOverlay(imguilayer);

	}
	
	Application::~Application() {}

	void Application::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		d.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

		for(auto it = layerstack.end(); it != layerstack.begin();)
		{
			if(e.handled)
				break;
			(*--it)->onEvent(e);
		}
	}

	void Application::run() 
	{
		while(running)
		{
			float time = static_cast<float>(glfwGetTime());
			Timestep ts(time - lastTime);
			lastTime = time;

			if(!minimized)
			{
				for(Layer* layer : layerstack)
				{
					layer->onUpdate(ts);
				}
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

	bool Application::onWindowResize(WindowResizeEvent& e) 
	{
		if(std::get<0>(e.getSize()) == 0 || std::get<1>(e.getSize()) == 0)
		{
			minimized = true;
			return false;
		}

		Renderer::onWindowResize(std::get<0>(e.getSize()), std::get<1>(e.getSize()));
		minimized = false;
		return false;
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