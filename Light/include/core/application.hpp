#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "core/window.hpp"
#include "events/applicationevent.hpp"
#include "core/layerstack.hpp"
#include "imgui/imguilayer.hpp"

//Temp
#include "glad/glad.h"

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

		inline Window& getWindow() { return *window; }

		static inline Application& get() { return *instance; }

		inline void close() { running = false; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		bool running = true;
		bool minimized = false;

		std::unique_ptr<Window> window;
		ImguiLayer* imguilayer;
		LayerStack layerstack;

		static Application* instance;

		float lastTime = 0.0f;
	};

	Application* createApplication();
}
#endif // __APPLICATION_H__