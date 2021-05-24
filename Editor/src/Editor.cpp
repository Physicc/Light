#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "Objects.hpp"
#include "Scene.hpp"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), 
					scene(
						{45.0f, 1.6f/0.9f, 0.001f, 100.0f},
						{-1,2,1.5},
						{glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(2,0.1,2)}
					)
	{
		scene.object_init();
		Light::FramebufferSpec fbspec;
		fbspec.width = 1280;
		fbspec.height = 720;
        m_framebuffer = Light::Framebuffer::create(fbspec);
	}
	~ExampleLayer() = default;

	void onUpdate(Light::Timestep ts) override
	{
		if(m_resizeViewport)
		{
			scene.camResize(viewportPanelSize.x, viewportPanelSize.y);
			framebuffer->resize(viewportPanelSize.x, viewportPanelSize.y);
			resizeViewport = false;
		}
		m_frameCount++;
        m_time += ts.getMilliSeconds();
		if(m_time >= 500.0f)
		{
            m_lastTime = m_time;
            m_lastFrameCount = m_frameCount;
            m_time = 0.0f;
            m_frameCount = 0;
		}

		scene.onUpdate(ts);

		m_framebuffer->bind();

		scene.render();

		m_framebuffer->unbind();
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto[width, height] = e.getSize();
		
		scene.camResize(width,height);

		return false;
	}


	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResize));

		scene.onEvent(e);
	}

	void onImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		Light::Application::get().getImguiLayer()->blockFocusEvents(!ImGui::IsWindowFocused());
		Light::Application::get().getImguiLayer()->blockHoverEvents(!ImGui::IsWindowHovered());

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if(m_viewportPanelSize != *(glm::vec2*)&panelSize)
		{
            m_resizeViewport = true;
            m_viewportPanelSize = glm::vec2(panelSize.x, panelSize.y);
		}
		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId()), panelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Scene Settings");
		ImGui::DragFloat3("Light Position", scene.getLightPos(), 0.01f);
		ImGui::End();

		ImGui::Begin("Camera Controls");
		ImGui::Text("Left Alt + LMB to Orbit");
		ImGui::Text("Left Alt + MMB to Pan");
		ImGui::Text("Left Alt + RMB to Zoom");
		ImGui::Text("Scroll to Zoom");
		ImGui::End();

		ImGui::Begin("Performance Statistics");
		ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d",
                    m_lastTime / m_lastFrameCount,
                    m_lastTime * 0.001f / m_lastFrameCount,
                    int(m_lastFrameCount * 1000 / m_lastTime));
		ImGui::End();

	}

private:
	Scene scene;

	std::shared_ptr<Light::Framebuffer> framebuffer;

	glm::vec2 viewportPanelSize;
	bool resizeViewport = false;

	float time = 0.0f;
	int frameCount = 0;
	float lastTime = 0.0f;
	int lastFrameCount = 0;
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() = default;
};

Light::Application* Light::createApplication()
{
	return new Editor();
}