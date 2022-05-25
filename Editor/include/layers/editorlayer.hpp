#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include "light.hpp"
#include "gui/scenepanel.hpp"
#include "gui/projectnamepopup.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

namespace Light
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		void onUpdate(Timestep ts) override;

		bool onWindowResize(WindowResizeEvent& e);

		bool onMouseButtonPressed(MouseButtonPressedEvent& e);

		bool onKeyPressed(KeyPressedEvent& e);

		void onEvent(Event& e) override;

		void onImguiRender() override;

		void addDefaultMeshes();

	private:
		std::shared_ptr<MeshLibrary> m_meshes;

		SceneRenderer m_sceneRenderer;
		EditorCamera m_camera;

		std::shared_ptr<Framebuffer> m_framebuffer;

		std::shared_ptr<Scene> m_scene;
		ScenePanel m_scenePanel;

		Entity m_hoveredEntity;

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
		ProjectNamePopup m_projectNamePopup;
	};
}


#endif // __EDITORLAYER_H__
