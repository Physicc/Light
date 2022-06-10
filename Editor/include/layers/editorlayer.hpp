#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include "light.hpp"
#include "gui/scenepanel.hpp"
#include "gui/projectnamepopup.hpp"
#include "gui/viewportpanel.hpp"
#include "gui/assetbrowser.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

namespace Light
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer() = default;

		void onAttach() override;

		void onUpdate(Timestep ts) override;
		void onEvent(Event& e) override;
		void onImguiRender() override;


	private:
		bool onWindowResize(WindowResizeEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);

		void addDefaultMeshes();

		std::shared_ptr<MeshLibrary> m_meshes;

		SceneRenderer m_sceneRenderer;
		EditorCamera m_camera;

		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Framebuffer> m_framebuffer;

		// GUI Panels
		ScenePanel m_scenePanel;
		ProjectNamePopup m_projectNamePopup;
		ViewportPanel m_viewportPanel;
		AssetBrowser m_assetBrowser;
	};
}


#endif // __EDITORLAYER_H__
