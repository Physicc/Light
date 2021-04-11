#include "core/application.hpp"

#include "rendering/rendercommand.hpp"
#include "core/timestep.hpp"

//Temp for time
#include "glfw/glfw3.h"

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
			float time = glfwGetTime();
			Timestep ts(time - lastTime);
			lastTime = time;

			RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
			RenderCommand::clear();
			
			for(Layer* layer : layerstack)
			{
				layer->onUpdate(ts);
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