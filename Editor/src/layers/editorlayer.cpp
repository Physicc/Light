#include "layers/editorlayer.hpp"

#include <filesystem>

#include "core/logging.hpp"

namespace Light
{
	EditorLayer::EditorLayer() : Layer("MainLayer"),
						m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f)
	{
	}

	void EditorLayer::onAttach()
	{
		// Check if project_path exists in config
		if (!Application::get().getConfig().Has("project_path"))
		{
			m_projectNamePopup.openPopup();
		}

		// Initialize framebuffer
		FramebufferSpec fbspec;
		fbspec.attachments = {
			{ FramebufferTextureFormat::RGBA8, TextureWrap::CLAMP_TO_BORDER },
			{ FramebufferTextureFormat::RED_INTEGER, TextureWrap::CLAMP_TO_BORDER },
			{ FramebufferTextureFormat::Depth, TextureWrap::CLAMP_TO_BORDER }
		};
		fbspec.width = 1280;
		fbspec.height = 720;
		m_framebuffer = Framebuffer::create(fbspec);

		// Set project popup callback
		m_projectNamePopup.setInputCallback([](const std::string& path)
		{
			Application::get().getConfig().SetString("project_path", path);
			Application::get().getConfig().Save(Application::get().getConfigPath().string());
			LIGHT_CORE_DEBUG("Project path set to {0}", path);
		});

		// Set viewport panel resize callback
		m_viewportPanel.setViewportResizeCallback([this](int width, int height)
		{
			m_camera.setViewportSize(width, height);
			m_sceneRenderer.onViewportResize(width, height);
			m_framebuffer->resize(width, height);
		});

		// Set viewport panel mouse button callback
		m_viewportPanel.setMouseButtonPressedCallback([this](int button, int x, int y)
		{
			if (button == LIGHT_MOUSE_BUTTON_LEFT)
			{
				m_framebuffer->bind();

				int pixelData = m_framebuffer->readPixelInt(1, x, m_framebuffer->getSpec().height - y);

				m_framebuffer->unbind();

				auto selectedEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_scene.get());

				m_scenePanel.setSelectionContext(selectedEntity);
			}
		});

		m_viewportPanel.setCamera(&m_camera);

		m_sceneRenderer.setTargetFramebuffer(m_framebuffer);
		m_viewportPanel.setFramebuffer(m_framebuffer);

		m_scene = std::make_shared<Scene>();

		auto cube = m_scene->addEntity("Cube");
		cube.addComponent<MeshRendererComponent>("assets/shaders/phong.glsl");

		auto floor = m_scene->addEntity("Floor");
		auto& floor_transform = floor.getComponent<TransformComponent>();
		floor_transform.position = glm::vec3(0, -1, 0);
		floor_transform.scale = glm::vec3(2, 0.1, 2);
		floor.addComponent<MeshRendererComponent>("assets/shaders/phong.glsl");

		auto light = m_scene->addEntity("Light");
		auto& light_transform = light.getComponent<TransformComponent>();
		light_transform.position = glm::vec3(-1,2,1.5);
		light.addComponent<LightComponent>();

		m_meshes = std::make_shared<MeshLibrary>();

		addDefaultMeshes();

		cube.addComponent<MeshComponent>(m_meshes->get("Cube"));
		floor.addComponent<MeshComponent>(m_meshes->get("Cube"));

		m_scenePanel.setContext(m_scene);
		m_scenePanel.setMeshLibrary(m_meshes);
	}

	void EditorLayer::onUpdate(Timestep ts)
	{
		auto selectedEntity = m_scenePanel.getSelectionContext();
		m_viewportPanel.setSelectionContext(selectedEntity);
		m_viewportPanel.onUpdate();

		m_assetBrowser.onUpdate(ts);

		if (!m_viewportPanel.isViewportFocused() || m_viewportPanel.isOverGizmo() || m_viewportPanel.isUsingGizmo())
		{
			m_camera.blockUpdate(true);
		}
		m_camera.onUpdate(ts);

		m_sceneRenderer.renderEditor(m_scene, m_camera);
		m_sceneRenderer.renderOutline(m_scene, selectedEntity);
	}

	bool EditorLayer::onWindowResize(WindowResizeEvent&)
	{
		return false;
	}

	bool EditorLayer::onMouseButtonPressed([[maybe_unused]] MouseButtonPressedEvent& e)
	{

		return false;
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		switch (e.getKeycode())
		{
		case LIGHT_KEY_O:
			if (Input::isKeyPressed(LIGHT_KEY_LEFT_CONTROL)) { m_projectNamePopup.openPopup(); }
			break;
		default:
			break;
		}

		return false;
	}

	void EditorLayer::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorLayer::onWindowResize));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::onKeyPressed));
		m_camera.onEvent(e);
		m_viewportPanel.onEvent(e);
		m_assetBrowser.onEvent(e);
	}

	void EditorLayer::onImguiRender()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Project Popup
		m_projectNamePopup.onImguiRender();

		// Main Menu Bar
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Application::get().close();
				if(ImGui::MenuItem("Open Project", "Ctrl+O")) m_projectNamePopup.openPopup();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		static double fps = 0, mspf = 0;
		static int frameCount = 0;
		frameCount++;
		if(frameCount > 60)
		{
			frameCount = 0;
			fps = Application::get().getStats().m_fps;
			mspf = Application::get().getStats().m_mspf;
		}

		// Performance Stats
		{
			ImGui::Begin("Performance Stats");
			ImGui::Text("FPS: %.2f", fps);
			ImGui::Text("Frame Time: %.2f ms", mspf);
			ImGui::End();
		}

		m_viewportPanel.onImguiRender();

		m_scenePanel.onImguiRender();

		m_assetBrowser.onImguiRender();
	}

	void EditorLayer::addDefaultMeshes()
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

}
