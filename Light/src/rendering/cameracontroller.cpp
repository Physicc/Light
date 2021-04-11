#include "rendering/cameracontroller.hpp"

#include "core/input.hpp"
#include "input/keycodes.h"

namespace Light
{
	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float near, float far, 
															glm::vec3 lookAtDirection, glm::vec3 upDirection) 
		:	fovy(45.0f),
			aspectRatio(aspectRatio), 
			near(near),
			far(far),
			lookAtDirection(lookAtDirection),
			upDirection(upDirection),
			cameraPositionSpeed(10.0f),
			camera(fovy, aspectRatio, near, far)
	{
		position = camera.getPosition();
	}
	
	void PerspectiveCameraController::onUpdate(Timestep ts) 
	{
		if(Input::isKeyPressed(LIGHT_KEY_W))
		{
			position.z += cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_S))
		{
			position.z -= cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_A))
		{
			position.x -= cameraPositionSpeed * ts.getSeconds();
		}
		else if(Input::isKeyPressed(LIGHT_KEY_D))
		{
			position.x += cameraPositionSpeed * ts.getSeconds();
		}

		camera.setPosition(position);
	}

	void PerspectiveCameraController::onEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResized));
	}

	bool PerspectiveCameraController::onMouseScrolled(MouseScrolledEvent& e) 
	{
		fovy += std::get<1>(e.getOffset());
		camera.setProjection(fovy, aspectRatio, near, far);
		return false;
	}
	
	bool PerspectiveCameraController::onWindowResized(WindowResizeEvent& e) 
	{
		return false;
	}
}