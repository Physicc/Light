#ifndef __CAMERACONTROLLER_H__
#define __CAMERACONTROLLER_H__

#include "core/base.hpp"

#include "rendering/camera.hpp"
#include "core/timestep.hpp"
#include "events/event.hpp"
#include "events/keyevent.hpp"
#include "events/mouseevent.hpp"
#include "events/applicationevent.hpp"

namespace Light
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float aspectRatio, float near, float far,
									glm::vec3 lookAtDirection, glm::vec3 upDirection);

		void onUpdate(Timestep ts);

		void onEvent(Event& e);

		PerspectiveCamera& getCamera() { return camera; }
		const PerspectiveCamera& getCamera() const { return camera; }

	private:

		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

		float aspectRatio;
		float fovy;
		float near;
		float far;

		glm::vec3 position;
		glm::vec3 lookAtDirection;
		glm::vec3 upDirection;

		float cameraPositionSpeed;

		PerspectiveCamera camera;
	};

}

#endif // __CAMERACONTROLLER_H__