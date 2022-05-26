#include "gui/viewportpanel.hpp"

#include "core/application.hpp"

#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"
#include "ecs/components.hpp"

#include "core/logging.hpp"

namespace Light
{
	void ViewportPanel::getViewportProperties()
	{
		if(ImGui::IsWindowHovered() || ImGui::IsWindowFocused())
		{
			ImGui::CaptureMouseFromApp(false);
		}
		m_viewportFocused = ImGui::IsWindowFocused();

		ImVec2 viewportPos = ImGui::GetCursorScreenPos();
		m_viewportPos = { viewportPos.x, viewportPos.y };

		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		if(m_viewportSize.x != viewPortPanelSize.x || m_viewportSize.y != viewPortPanelSize.y)
		{
			m_resizeViewport = true;
			m_viewportSize = glm::vec2(viewPortPanelSize.x, viewPortPanelSize.y);
		}

		auto mousePos = ImGui::GetMousePos();
		m_mousePos = { mousePos.x, mousePos.y };
	}

	void ViewportPanel::onImguiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		getViewportProperties();

		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId(0)), {m_viewportSize.x, m_viewportSize.y}, ImVec2(0, 1), ImVec2(1, 0));

		onGizmoRender();

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void ViewportPanel::onGizmoRender()
	{
		ImGuizmo::BeginFrame();
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(m_viewportPos.x, m_viewportPos.y, m_viewportSize.x, m_viewportSize.y);

		auto cameraViewMatrix = m_camera->getViewMatrix();

		if (m_selectionContext)
		{
			auto& transformComponent = m_selectionContext.getComponent<TransformComponent>();
			glm::mat4 transform = transformComponent.getTransform();


			bool snap = ImGui::IsKeyDown(LIGHT_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_gizmo_operation == ImGuizmo::OPERATION::ROTATE)
			{
				snapValue = 15.0f;
			}
			float snapVector[3] = {snapValue, snapValue, snapValue};

			ImGuizmo::Manipulate(glm::value_ptr(cameraViewMatrix), glm::value_ptr(m_camera->getProjectionMatrix()),
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

		float manipulateWidgetSize = glm::max(m_viewportSize.x / 7, m_viewportSize.y / 7);

		if(ImGuizmo::ViewManipulate(glm::value_ptr(cameraViewMatrix), m_camera->getDistance(),
			{m_viewportPos.x + m_viewportSize.x - manipulateWidgetSize, m_viewportPos.y}, {manipulateWidgetSize, manipulateWidgetSize}, 0x10101010))
		{
			m_camera->setViewMatrix(cameraViewMatrix);
		}
	}

	void ViewportPanel::onUpdate()
	{

		if (m_resizeViewport)
		{
			m_resizeViewport = false;
			if (m_viewportResizeCallback)
			{
				m_viewportResizeCallback((int)m_viewportSize.x, (int)m_viewportSize.y);
			}
		}
	}

	bool ViewportPanel::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (!m_gizmoOver && !m_gizmoUsing)
		{
			glm::vec2 relativePos = m_mousePos - m_viewportPos;

			if (m_mousePos.x >= m_viewportPos.x && m_mousePos.x <= m_viewportPos.x + m_viewportSize.x &&
				m_mousePos.y >= m_viewportPos.y && m_mousePos.y <= m_viewportPos.y + m_viewportSize.y)
			{
				m_mouseButtonPressedCallback(e.getButton(), (int)relativePos.x, (int)relativePos.y);
			}
		}

		return false;
	}

	bool ViewportPanel::onKeyPress(KeyPressedEvent& e)
	{
		if(m_selectionContext)
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

	void ViewportPanel::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ViewportPanel::onMouseButtonPressed));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ViewportPanel::onKeyPress));
	}
}
