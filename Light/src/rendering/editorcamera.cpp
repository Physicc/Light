#include "rendering/editorcamera.hpp"

#include "core/input.hpp"
#include "input/keycodes.h"
#include "input/mousecodes.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace Light
{
	EditorCamera::EditorCamera(float fovy, float aspectRatio, float near, float far)
		:	fovy(fovy),
			aspectRatio(aspectRatio), 
			near(near),
			far(far)
	{
		updateView();
		updateProjection();
	}

	void EditorCamera::setViewportSize(int width, int height)
	{
		viewportWidth = width;
		viewportHeight = height;
		aspectRatio = float(width)/height;
		updateProjection();
	}

	void EditorCamera::updateView() 
	{
		position = calculatePosition();

		glm::quat orientation = getOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	void EditorCamera::updateProjection()
	{
		setProjectionMatrix(glm::perspective(glm::radians(fovy), aspectRatio, near, far));
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
		float x = std::min(viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::rotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::zoomSpeed() const
	{
		float distance_ = distance * 0.2f;
		distance_ = std::max(distance_, 0.0f);
		float speed = distance_ * distance_;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}
	
	void EditorCamera::onUpdate(Timestep ts) 
	{
		if (Input::isKeyPressed(LIGHT_KEY_LEFT_ALT))
		{
			auto[mouseX, mouseY] = Input::getMousePos();
			const glm::vec2& mouse{ mouseX, mouseY };
			glm::vec2 delta = (mouse - initialMousePos) * 0.003f;
			initialMousePos = mouse;

			if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_MIDDLE))
				mousePan(delta);
			else if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_LEFT))
				mouseRotate(delta);
			else if (Input::isMouseButtonPressed(LIGHT_MOUSE_BUTTON_RIGHT))
				mouseZoom(delta.y);
		}

		updateView();
	}

	void EditorCamera::onEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::onMouseScrolled));
	}

	bool EditorCamera::onMouseScrolled(MouseScrolledEvent& e)
	{
		auto[xOffset, yOffset] = e.getOffset();
		float delta = float(yOffset) * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		focalPoint += -getRightDirection() * delta.x * xSpeed * distance;
		focalPoint += getUpDirection() * delta.y * ySpeed * distance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * rotationSpeed();
		pitch += delta.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		distance -= delta * zoomSpeed();
		if (distance < 1.0f)
		{
			focalPoint += getForwardDirection();
			distance = 1.0f;
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
		return focalPoint - getForwardDirection() * distance;
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}
}