#include "rendering/editorcamera.hpp"

#include "core/input.hpp"
#include "input/keycodes.h"

namespace Light
{
	EditorCamera::EditorCamera(float fovy, float aspectRatio, float near, float far)
		:	fovy(fovy),
			aspectRatio(aspectRatio), 
			near(near),
			far(far),
			cameraPositionSpeed(0.5f),
			mousePressed(false),
			ctrlPressed(false),
			position(glm::vec3(-0.754, 0.651, 1.758)),
			lookAtDirection(glm::vec3(0.35, -0.3, -0.9)),
			upDirection(glm::vec3(0.0,1.0,0.0))
	{
		updateViewMatrix();
		setProjectionMatrix(glm::perspective(fovy, aspectRatio, near, far));
	}

	void EditorCamera::updateViewMatrix() 
	{
		viewMatrix = glm::lookAt(position, position + lookAtDirection, upDirection);
	}
	
	void EditorCamera::onUpdate(Timestep ts) 
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

		updateViewMatrix();
	}

	void EditorCamera::onEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorCamera::onWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorCamera::onMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(EditorCamera::onMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(EditorCamera::onMouseMoved));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorCamera::onKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(EditorCamera::onKeyReleased));
	}

	bool EditorCamera::onMouseScrolled(MouseScrolledEvent& e) 
	{
		fovy = std::max(fovy - std::get<1>(e.getOffset()), 20.0);
		setProjectionMatrix(glm::perspective(fovy, aspectRatio, near, far));
		return false;
	}

	bool EditorCamera::onMouseButtonPressed(MouseButtonPressedEvent& e) 
	{
		mousePressed = true;
		return false;
	}

	bool EditorCamera::onMouseButtonReleased(MouseButtonReleasedEvent& e) 
	{
		mousePressed = false;
		return false;
	}

	bool EditorCamera::onKeyPressed(KeyPressedEvent& e) 
	{
		if(e.getKeycode() == LIGHT_KEY_LEFT_CONTROL)
		{
			ctrlPressed = true;
		}
		return false;
	}

	bool EditorCamera::onKeyReleased(KeyReleasedEvent& e) 
	{
		if(e.getKeycode() == LIGHT_KEY_LEFT_CONTROL)
		{
			ctrlPressed = false;
		}
		return false;
	}

	bool EditorCamera::onMouseMoved(MouseMovedEvent& e)
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
			}
			else // LMB + L.Ctrl + Mouse move: Camera pan 
			{
				double diffx = std::get<0>(e.getPos()) - std::get<0>(mousePos);
				double diffy = std::get<1>(e.getPos()) - std::get<1>(mousePos);

				glm::vec3 left = glm::normalize(glm::cross(upDirection, lookAtDirection));
				position += 0.001f * float(diffx) * left;
				position += 0.001f * float(diffy) * glm::normalize(upDirection);
			}
		}
		mousePos = e.getPos();

		updateViewMatrix();
		return false;
	}
	
	bool EditorCamera::onWindowResized(WindowResizeEvent& e) 
	{
		aspectRatio = float(std::get<0>(e.getSize()))/std::get<1>(e.getSize());
		setProjectionMatrix(glm::perspective(fovy, aspectRatio, near, far));
		return false;
	}
}