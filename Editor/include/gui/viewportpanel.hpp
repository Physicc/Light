#ifndef __VIEWPORTPANEL_H__
#define __VIEWPORTPANEL_H__

#include "light/rendering/framebuffer.hpp"
#include "events/event.hpp"
#include "events/mouseevent.hpp"
#include "glm/glm.hpp"
#include "ecs/entity.hpp"
#include "rendering/editorcamera.hpp"

#include "imgui.h"
#include "ImGuizmo.h"

namespace Light
{
	class ViewportPanel
	{
	public:
		ViewportPanel() = default;
		~ViewportPanel() = default;

		inline void setFramebuffer(std::shared_ptr<Framebuffer> framebuffer) { m_framebuffer = framebuffer; }

		void onImguiRender();
		void onUpdate();
		void onEvent(Event& e);


		inline void setViewportResizeCallback(std::function<void(int width, int height)> callback) { m_viewportResizeCallback = callback; }
		inline void setMouseButtonPressedCallback(std::function<void(int button, int x, int y)> callback) { m_mouseButtonPressedCallback = callback; }
		inline void setSelectionContext(Entity context) { m_selectionContext = context; }
		inline void setCamera(EditorCamera* camera) { m_camera = camera; }

		inline bool isUsingGizmo() const { return m_gizmoUsing; }
		inline bool isOverGizmo() const { return m_gizmoOver; }
		inline bool isViewportFocused() const { return m_viewportFocused; }

	private:

		void getViewportProperties();
		void onGizmoRender();

		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onKeyPress(KeyPressedEvent& e);

		std::function<void(int, int)> m_viewportResizeCallback;
		std::function<void(int, int, int)> m_mouseButtonPressedCallback;

		glm::vec2 m_viewportSize;
		glm::vec2 m_viewportPos;
		glm::vec2 m_mousePos;
		bool m_resizeViewport = false;
		bool m_viewportFocused = false;

		ImGuizmo::OPERATION m_gizmo_operation = ImGuizmo::TRANSLATE;
		bool m_gizmoUsing = false;
		bool m_gizmoOver = false;

		Entity m_selectionContext;
		EditorCamera* m_camera;

		std::shared_ptr<Framebuffer> m_framebuffer;
	};
} // namespace Light


#endif // __VIEWPORTPANEL_H__
