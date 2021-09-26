#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "core/window.hpp"
#include "events/applicationevent.hpp"
#include "core/layerstack.hpp"
#include "imgui/imguilayer.hpp"

namespace Light
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void onEvent(Event& e);
		void run();
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		inline Window& getWindow() { return *m_window; }

		static inline Application& get() { return *m_instance; }

		ImguiLayer* getImguiLayer() { return m_imguiLayer; }

		inline void close() { m_running = false; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		bool m_running = true;
		bool m_minimized = false;

		std::unique_ptr<Window> m_window;
		ImguiLayer* m_imguiLayer;
		LayerStack m_layerStack;

		static Application* m_instance;

		float m_lastTime = 0.0f;
	};

	Application* createApplication();
}
#endif // __APPLICATION_H__