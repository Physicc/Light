#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include "light.hpp"
#include "gui/scenepanel.hpp"
#include "gui/projectnamepopup.hpp"
#include "gui/viewportpanel.hpp"

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
		void onEvent(Event& e) override;
		void onImguiRender() override;


	private:
		bool onWindowResize(WindowResizeEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);

		void addDefaultMeshes();

		std::string m_configPath = "editorconfig.json";

		std::shared_ptr<MeshLibrary> m_meshes;

		SceneRenderer m_sceneRenderer;
		EditorCamera m_camera;

		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Framebuffer> m_framebuffer;

		float m_time = 0.0f;
		int m_frameCount = 0;
		float m_lastTime = 0.0f;
		int m_lastFrameCount = 0;

		// GUI Panels
		ScenePanel m_scenePanel;
		ProjectNamePopup m_projectNamePopup;
		ViewportPanel m_viewportPanel;
	};
}


#endif // __EDITORLAYER_H__
