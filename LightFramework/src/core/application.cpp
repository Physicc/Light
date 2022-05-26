#include "tools/Tracy.hpp"

#include "core/application.hpp"

#include "light/rendering/renderer.hpp"

#include "core/logging.hpp"

//Temp for time
#include "GLFW/glfw3.h"

namespace Light
{
	Application* Application::m_instance = nullptr;

	Application::Application()
	{
		if(m_instance == nullptr)
            m_instance = this;
		else
		{
			LIGHT_CORE_CRITICAL("Application already created");
			exit(1);
		}
        m_window = static_cast<std::unique_ptr<Window>>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		Renderer::init();
		LIGHT_CORE_INFO("Initialized Renderer");

        m_imguiLayer = new ImguiLayer("ImGui Layer");
		pushOverlay(m_imguiLayer);

	}

	Application::~Application() = default;

	void Application::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
		d.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

		for(auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			if(e.handled)
				break;
			(*--it)->onEvent(e);
		}
	}

	void Application::run()
	{
		while(m_running)
		{
			static Timer deltaTimer;
			double dt = deltaTimer.getDeltaTime<std::chrono::milliseconds>();
			deltaTimer.start();

			m_stats.m_mspf = dt;
			m_stats.m_fps = 1000.0 / dt * m_stats.m_alpha + m_stats.m_fps * (1 - m_stats.m_alpha);

			if(!m_minimized)
			{
				for(Layer* layer : m_layerStack)
				{
					layer->onUpdate(Timestep(dt	/ 1000.0f));
				}
			}

			m_imguiLayer->begin();
			for(Layer* layer : m_layerStack)
			{
				layer->onImguiRender();
			}
			m_imguiLayer->end();

			m_window->onUpdate();

			FrameMark;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent&)
	{
        m_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		if(std::get<0>(e.getSize()) == 0 || std::get<1>(e.getSize()) == 0)
		{
            m_minimized = true;
			return false;
		}

		Renderer::onWindowResize(std::get<0>(e.getSize()), std::get<1>(e.getSize()));
        m_minimized = false;
		return false;
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_layerStack.pushOverlay(overlay);
		overlay->onAttach();
	}
}
