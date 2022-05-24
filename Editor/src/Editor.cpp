#include "gui/scenepanel.hpp"
#include "gui/projectnamepopup.hpp"

#include "physicsworld.hpp"
#include "imgui.h"
#include "light.hpp"
#include "core/entrypoint.hpp"
#include "ImGuizmo.h"

#include <filesystem>


class MainLayer : public Light::Layer
{
public:
	MainLayer(): Light::Layer("MainLayer"),
					m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f)
	{
		// Check if m_configPath exists
		if (std::filesystem::exists(m_configPath))
		{
			Light::Application::get().getConfig().Load(m_configPath);
		}

		Light::FramebufferSpec fbspec;
		fbspec.attachments = {
			{ Light::FramebufferTextureFormat::RGBA8, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::RED_INTEGER, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspec.width = 1280;
		fbspec.height = 720;
		m_framebuffer = Light::Framebuffer::create(fbspec);

		m_sceneRenderer.setTargetFramebuffer(m_framebuffer);

		m_scene = std::make_shared<Light::Scene>();

		auto cube = m_scene->addEntity("Cube");
		cube.addComponent<Light::MeshRendererComponent>("assets/shaders/phong.glsl");

		auto floor = m_scene->addEntity("Floor");
		auto& floor_transform = floor.getComponent<Light::TransformComponent>();
		floor_transform.position = glm::vec3(0, -1, 0);
		floor_transform.scale = glm::vec3(2, 0.1, 2);
		floor.addComponent<Light::MeshRendererComponent>("assets/shaders/phong.glsl");

		auto light = m_scene->addEntity("Light");
		auto& light_transform = light.getComponent<Light::TransformComponent>();
		light_transform.position = glm::vec3(-1,2,1.5);
		light.addComponent<Light::LightComponent>();

		m_meshes = std::make_shared<Light::MeshLibrary>();

		addDefaultMeshes();

		cube.addComponent<Light::MeshComponent>(m_meshes->get("Cube"));
		floor.addComponent<Light::MeshComponent>(m_meshes->get("Cube"));

		m_scenePanel.setContext(m_scene);
		m_scenePanel.setMeshLibrary(m_meshes);
	}
	~MainLayer() = default;

	void onUpdate(Light::Timestep ts) override
	{
		if(m_resizeViewport)
		{
			m_camera.setViewportSize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
			m_sceneRenderer.onViewportResize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
			m_framebuffer->resize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
			m_resizeViewport = false;
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

		if(m_viewportFocused && !m_gizmoOver && !m_gizmoUsing)
			m_camera.onUpdate(ts);

		m_sceneRenderer.renderEditor(m_scene, m_camera);

		m_framebuffer->bind();
		auto[x, y] = ImGui::GetMousePos();

		glm::vec2 posRelativeToViewport = glm::vec2(x,y) - m_viewportPos;

		if(posRelativeToViewport.x >= 0 && posRelativeToViewport.y >= 0
			&& posRelativeToViewport.x < m_viewportPanelSize.x && posRelativeToViewport.y < m_viewportPanelSize.y)
		{
			int pixelData = m_framebuffer->readPixelInt(1, int(posRelativeToViewport.x), int(m_viewportPanelSize.y - posRelativeToViewport.y));

			m_hoveredEntity = pixelData == -1 ? Light::Entity() : Light::Entity((entt::entity)pixelData, m_scene.get());
		}

		m_framebuffer->unbind();

		auto entity = m_scenePanel.getSelectionContext();
		m_sceneRenderer.renderOutline(m_scene, entity);

	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto[width, height] = e.getSize();
		if(width == 0 || height == 0)
		{
			return false;
		}
		m_camera.setViewportSize(width, height);
		return false;
	}

	bool onMouseButtonPressed(Light::MouseButtonPressedEvent& e)
	{
		if(e.getButton() == LIGHT_MOUSE_BUTTON_LEFT && !m_gizmoOver)
		{
			m_scenePanel.setSelectionContext(m_hoveredEntity);
		}

		return false;
	}

	bool onKeyPressed(Light::KeyPressedEvent& e)
	{
		if(m_scenePanel.getSelectionContext())
		{
			switch (e.getKeycode())
			{
			case LIGHT_KEY_W:
				m_gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case LIGHT_KEY_E:
				m_gizmo_operation = ImGuizmo::OPERATION::ROTATE;
				break;
			case LIGHT_KEY_R:
				m_gizmo_operation = ImGuizmo::OPERATION::SCALE;
				break;
			case LIGHT_KEY_T:
				m_gizmo_operation = ImGuizmo::OPERATION::UNIVERSAL;
				break;

			default:
				break;
			}

			return true;
		}

		return false;
	}


	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(MainLayer::onWindowResize));
		dispatcher.Dispatch<Light::MouseButtonPressedEvent>(BIND_EVENT_FN(MainLayer::onMouseButtonPressed));
		dispatcher.Dispatch<Light::KeyPressedEvent>(BIND_EVENT_FN(MainLayer::onKeyPressed));

		m_camera.onEvent(e);
	}

	void onImguiRender() override
	{
		static int s_stats_corner = 0;
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Demo
		// ImGui::ShowDemoWindow();

		// Project Popup
		if(m_projectNamePopup.onImguiRender())
		{
			Light::Application::get().getConfig().Save(m_configPath);
			LIGHT_CORE_DEBUG("Saved config to {}", m_configPath);
		}

		// Main Menu Bar
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Settings"))
			{
				if(ImGui::MenuItem("Show stats", NULL, s_stats_corner != -1))
					s_stats_corner = (s_stats_corner == -1) ? 0 : -1;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		ImVec2 viewPortBounds[] = {
			{viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y},
			{viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y}
		};

		ImVec2 viewportPos = ImGui::GetCursorScreenPos();
		m_viewportPos = { viewportPos.x, viewportPos.y };

		bool viewportDocked = ImGui::IsWindowDocked();

		Light::Application::get().getImguiLayer()->blockFocusEvents(!ImGui::IsWindowFocused());
		Light::Application::get().getImguiLayer()->blockHoverEvents(!ImGui::IsWindowHovered());

		m_viewportFocused = ImGui::IsWindowFocused();

		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		if(m_viewportPanelSize.x != viewPortPanelSize.x || m_viewportPanelSize.y != viewPortPanelSize.y)
		{
			m_resizeViewport = true;
			m_viewportPanelSize = glm::vec2(viewPortPanelSize.x, viewPortPanelSize.y);
		}

		// Gizmo
		ImGuizmo::BeginFrame();
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(viewPortBounds[0].x, viewPortBounds[0].y, viewPortBounds[1].x - viewPortBounds[0].x, viewPortBounds[1].y - viewPortBounds[0].y);

		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId(0)), viewPortPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		auto selectedEntity = m_scenePanel.getSelectionContext();
		auto cameraViewMatrix = m_camera.getViewMatrix();

		if (selectedEntity)
		{
			auto& transformComponent = selectedEntity.getComponent<Light::TransformComponent>();
			glm::mat4 transform = transformComponent.getTransform();


			bool snap = ImGui::IsKeyDown(LIGHT_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_gizmo_operation == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 15.0f;
			}
			float snapVector[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraViewMatrix), glm::value_ptr(m_camera.getProjectionMatrix()),
				m_gizmo_operation, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapVector : nullptr);

			m_gizmoUsing = ImGuizmo::IsUsing();
			m_gizmoOver = ImGuizmo::IsOver();

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(position), glm::value_ptr(rotation), glm::value_ptr(scale));
				transformComponent.position = position;
				transformComponent.rotation = glm::radians(rotation);
				transformComponent.scale = scale;
			}
		}

		if(ImGuizmo::ViewManipulate(glm::value_ptr(cameraViewMatrix), m_camera.getDistance(), {viewPortBounds[1].x - 128, viewPortBounds[0].y}, {128, 128}, 0x10101010))
		{
			m_camera.setViewMatrix(cameraViewMatrix);
		}

		ImGui::End();
		ImGui::PopStyleVar();

		// Perf Stats
		{
			// ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (s_stats_corner != -1)
			{
				const float PAD = 10.0f;
				ImVec2 work_pos = viewportPos;
				ImVec2 work_size = viewPortPanelSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (s_stats_corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (s_stats_corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (s_stats_corner & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (s_stats_corner & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

				ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
				if(viewportDocked)
				{
					ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
					window_flags |= ImGuiWindowFlags_NoMove;
				}
				if (ImGui::Begin("Performance Statistics", NULL, window_flags))
				{
					ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d",
						m_lastTime / m_lastFrameCount,
						m_lastTime * 0.001f / m_lastFrameCount,
						int(m_lastFrameCount * 1000 / m_lastTime));
					ImGui::Separator();
					ImGui::Text("(Right-Click to change position)");
					if (ImGui::BeginPopupContextWindow())
					{
						if (ImGui::MenuItem("Hide", NULL, s_stats_corner == -1)) s_stats_corner = -1;
						if(viewportDocked)
						{
							if (ImGui::MenuItem("Top-left",     NULL, s_stats_corner == 0)) s_stats_corner = 0;
							if (ImGui::MenuItem("Top-right",    NULL, s_stats_corner == 1)) s_stats_corner = 1;
							if (ImGui::MenuItem("Bottom-left",  NULL, s_stats_corner == 2)) s_stats_corner = 2;
							if (ImGui::MenuItem("Bottom-right", NULL, s_stats_corner == 3)) s_stats_corner = 3;
						}
						ImGui::EndPopup();
					}
				}
				ImGui::End();
			}
		}


		// Scene Hierarchy Panel
		m_scenePanel.onImguiRender();

	}

	void addDefaultMeshes()
	{
		m_meshes->add("None", std::vector<glm::vec3>(), std::vector<glm::vec4>(), std::vector<glm::vec3>(), std::vector<unsigned int>());

		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f),

			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),

			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),

			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),

			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),

			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f)
		};

		std::vector<glm::vec4> color(24, {0.8f, 0.8f, 0.8f, 1.0f});

		std::vector<glm::vec3> normals = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),

			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f)
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		m_meshes->add("Cube", vertices, color, normals, indices);
	}

private:
	std::shared_ptr<Light::MeshLibrary> m_meshes;

	Light::SceneRenderer m_sceneRenderer;
	Light::EditorCamera m_camera;

	std::shared_ptr<Light::Framebuffer> m_framebuffer;

	std::shared_ptr<Light::Scene> m_scene;
	Light::ScenePanel m_scenePanel;

	Light::Entity m_hoveredEntity;

	glm::vec2 m_viewportPanelSize;
	glm::vec2 m_viewportPos;

	ImGuizmo::OPERATION m_gizmo_operation = ImGuizmo::TRANSLATE;
	bool m_gizmoUsing = false;
	bool m_gizmoOver = false;

	bool m_resizeViewport = false;
	bool m_viewportFocused = false;
	float m_time = 0.0f;
	int m_frameCount = 0;
	float m_lastTime = 0.0f;
	int m_lastFrameCount = 0;

	std::string m_configPath = "editorconfig.json";
	Light::ProjectNamePopup m_projectNamePopup;
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new MainLayer());
	}
	~Editor() = default;
};

Light::Application* Light::createApplication()
{
	return new Editor();
}
