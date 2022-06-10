#include <spdlog/spdlog.h>
#undef near
#undef far
/*
 * windows.h (in minwindef.h) for some retarded reason #defines near and far to be nothing
 * spdlog ends up including windows.h somewhere and that breaks the constructor
 */

#include "rendering/editorcamera.hpp"

#include "core/input.hpp"
#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Light
{
	EditorCamera::EditorCamera(float fovy, float aspectRatio, float near, float far)
		: m_aspectRatio(aspectRatio),
		  m_fovy(fovy),
		  m_near(near),
		  m_far(far)
	{
		updateView();
		updateProjection();
	}

	void EditorCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		m_aspectRatio = static_cast<float>(width) / height;
		updateProjection();
	}

	void EditorCamera::updateView()
	{
		m_position = calculatePosition();

		glm::quat orientation = getOrientation();
		m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
		m_viewMatrix = glm::inverse(m_viewMatrix);
	}

	void EditorCamera::updateProjection()
	{
		setProjectionMatrix(glm::perspective(glm::radians(m_fovy), m_aspectRatio, m_near, m_far));
	}

	void EditorCamera::recalculateOrientation()
	{

		auto invertedView = glm::inverse(m_viewMatrix);
		m_position = glm::vec3(invertedView[3]);

		const glm::vec3 direction = -glm::vec3(invertedView[2]);
		m_yaw   = glm::atan(direction.x, -direction.z);
		m_pitch = -glm::asin(direction.y);

		m_focalPoint = m_position + m_distance * getForwardDirection();
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return {xFactor, yFactor};
	}

	float EditorCamera::rotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::zoomSpeed() const
	{
		float distance_ = m_distance * 0.2f;
		distance_ = std::max(distance_, 0.0f);
		float speed = distance_ * distance_;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::onUpdate(Timestep)
	{
		auto [mouseX, mouseY] = Input::getMousePos();
		const glm::vec2& mouse{mouseX, mouseY};
		glm::vec2 delta = (mouse - m_initialMousePos) * 0.003f;
		m_initialMousePos = mouse;

		if (!m_blockUpdate && Input::isKeyPressed(LIGHT_KEY_LEFT_ALT))
		{
			if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_MIDDLE))
				mousePan(delta);
			else if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_LEFT))
				mouseRotate(delta);
			else if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_RIGHT))
				mouseZoom(delta.y);

			updateView();
		}

		m_blockUpdate = false;
	}

	void EditorCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::onMouseScrolled));
	}

	bool EditorCamera::onMouseScrolled(MouseScrolledEvent& e)
	{
		auto [xOffset, yOffset] = e.getOffset();
		float delta = static_cast<float>(yOffset) * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += getUpDirection() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * rotationSpeed();
		m_pitch += delta.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		m_distance -= delta * zoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += getForwardDirection();
			m_distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::calculatePosition() const
	{
		return m_focalPoint - getForwardDirection() * m_distance;
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
	}
}
