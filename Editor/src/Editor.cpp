#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "Objects.hpp"

#include <stdio.h>

#include "btBulletDynamicsCommon.h"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), 
			camera(45.0f, 1.6f/0.9f, 0.001f, 100.0f),
			lightPos(-1,2,1.5),
			floor(glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(2,0.1,2)),
			cube(glm::vec3(-1,2,0), glm::vec3(0,-30.0f,10.0f))
	{
		// Rest of initialization
		texture.reset(Light::Texture2D::create("../Light/assets/textures/check.png"));

		Light::FramebufferSpec fbspec;
		fbspec.width = 1280;
		fbspec.height = 720;

		framebuffer = Light::Framebuffer::create(fbspec);
	}

	~ExampleLayer()
	{
	}

	void onUpdate(Light::Timestep ts) override
	{
		if(resizeViewport)
		{
			camera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);
			framebuffer->resize(viewportPanelSize.x, viewportPanelSize.y);
			resizeViewport = false;
		}

		framecount++;
		time += ts.getMilliSeconds();
		if(time >= 500.0f)
		{
			lastTime = time;
			lastFramecount = framecount;
			time = 0.0f;
			framecount = 0;
		}

		camera.onUpdate(ts);
		cube.onUpdate(ts);

		framebuffer->bind();

		Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
		Light::RenderCommand::clear();

		Light::Renderer::beginScene(camera, lightPos);
		
		skybox.render();
		cube.render();
		floor.render();

		Light::Renderer::endScene();

		framebuffer->unbind();
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto[width, height] = e.getSize();
		camera.setViewportSize(width, height);

		return false;
	}


	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResize));

		camera.onEvent(e);

		cube.onEvent(e);
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
		if(viewportPanelSize != *(glm::vec2*)&panelSize)
		{
			resizeViewport = true;
			viewportPanelSize = glm::vec2(panelSize.x, panelSize.y);
		}
		ImGui::Image((void*)framebuffer->getColorAttachmentRendererId(), panelSize, ImVec2(0, 1), ImVec2(1,0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Scene Settings");
		ImGui::DragFloat3("Light Position", &(lightPos.x), 0.01f);
		ImGui::End();

		ImGui::Begin("Camera Controls");
		ImGui::Text("Left Alt + LMB to Orbit");
		ImGui::Text("Left Alt + MMB to Pan");
		ImGui::Text("Left Alt + RMB to Zoom");
		ImGui::Text("Scroll to Zoom");
		ImGui::End();

		ImGui::Begin("Performance Statistics");
		ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d", 
					lastTime/lastFramecount,
					lastTime*0.001f/lastFramecount,
					int(lastFramecount*1000/lastTime));
		ImGui::End();

	}

private:
	Light::EditorCamera camera;
	Cube cube;
	Cube floor;
	Skybox skybox;
	glm::vec3 lightPos;

	std::shared_ptr<Light::Framebuffer> framebuffer;
	std::shared_ptr<Light::Texture2D> texture;

	glm::vec2 viewportPanelSize;
	bool resizeViewport = false;

	float time = 0.0f;
	int framecount = 0;
	float lastTime = 0.0f;
	int lastFramecount = 0;

};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}