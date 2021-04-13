#include "rendering/cameracontroller.hpp"

#include "core/input.hpp"
#include "input/keycodes.h"

namespace Light
{
	PerspectiveCameraController::PerspectiveCameraController(float fovy, float aspectRatio, float near, float far) 
		:	fovy(fovy),
			aspectRatio(aspectRatio), 
			near(near),
			far(far),
			cameraPositionSpeed(0.5f),
			camera(fovy, aspectRatio, near, far),
			mousePressed(false),
			ctrlPressed(false)
	{
		position = camera.getPosition();
		lookAtDirection = camera.getLookAtDirection();
		upDirection = camera.getUpDirection();
	}
	
	void PerspectiveCameraController::onUpdate(Timestep ts) 
	{
		if(Input::isKeyPressed(LIGHT_KEY_LEFT))
		{
			auto left = glm::cross(upDirection, lookAtDirection);
			position += glm::normalize(left) * cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_RIGHT))
		{
			auto left = glm::cross(upDirection, lookAtDirection);
			position -= glm::normalize(left) * cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_UP))
		{
			position += glm::normalize(lookAtDirection) * cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_DOWN))
		{
			position -= glm::normalize(lookAtDirection) * cameraPositionSpeed * ts.getSeconds();
		}

		camera.setPosition(position);
	}

	void PerspectiveCameraController::onEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMoved));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(onKeyReleased));
	}

	bool PerspectiveCameraController::onMouseScrolled(MouseScrolledEvent& e) 
	{
		fovy = std::max(fovy - std::get<1>(e.getOffset()), 20.0);
		camera.setProjection(fovy, aspectRatio, near, far);
		return false;
	}

	bool PerspectiveCameraController::onMouseButtonPressed(MouseButtonPressedEvent& e) 
	{
		mousePressed = true;
		return false;
	}

	bool PerspectiveCameraController::onMouseButtonReleased(MouseButtonReleasedEvent& e) 
	{
		mousePressed = false;
		return false;
	}

	bool PerspectiveCameraController::onKeyPressed(KeyPressedEvent& e) 
	{
		if(e.getKeycode() == LIGHT_KEY_LEFT_CONTROL)
		{
			ctrlPressed = true;
		}
		return false;
	}

	bool PerspectiveCameraController::onKeyReleased(KeyReleasedEvent& e) 
	{
		if(e.getKeycode() == LIGHT_KEY_LEFT_CONTROL)
		{
			ctrlPressed = false;
		}
		return false;
	}

	bool PerspectiveCameraController::onMouseMoved(MouseMovedEvent& e)
	{
		if(mousePressed)
		{
			if(!ctrlPressed) // Only LMB + Mouse move: Camera rotate
			{
				double diffx = std::get<0>(e.getPos()) - std::get<0>(mousePos);
				double diffy = std::get<1>(e.getPos()) - std::get<1>(mousePos);
				
				glm::vec3 target = position + glm::normalize(lookAtDirection);
				glm::vec3 left = glm::normalize(glm::cross(upDirection, lookAtDirection));
				target += 0.001f * float(diffy) * glm::normalize(upDirection);
				target += 0.001f * float(diffx) * left;

				lookAtDirection = target - position;
				camera.setLookAtDirection(lookAtDirection);
			}
			else // LMB + L.Ctrl + Mouse move: Camera pan 
			{
				double diffx = std::get<0>(e.getPos()) - std::get<0>(mousePos);
				double diffy = std::get<1>(e.getPos()) - std::get<1>(mousePos);

				glm::vec3 left = glm::normalize(glm::cross(upDirection, lookAtDirection));
				position += 0.001f * float(diffx) * left;
				position += 0.001f * float(diffy) * glm::normalize(upDirection);

				camera.setPosition(position);
			}
		}
		mousePos = e.getPos();
		return false;
	}
	
	bool PerspectiveCameraController::onWindowResized(WindowResizeEvent& e) 
	{
		aspectRatio = float(std::get<0>(e.getSize()))/std::get<1>(e.getSize());
		camera.setProjection(fovy, aspectRatio, near, far);
		return false;
	}
}