#ifndef __EDITORCAMERA_H__
#define __EDITORCAMERA_H__

#include "core/base.hpp"

#include "rendering/camera.hpp"
#include "core/timestep.hpp"
#include "events/event.hpp"
#include "events/keyevent.hpp"
#include "events/mouseevent.hpp"
#include "events/applicationevent.hpp"

namespace Light
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(float fovy, float aspectRatio, float near, float far);

		void onUpdate(Timestep ts);

		void onEvent(Event& e);

		inline void setAspectRatio(float aspectRatio)
		{ 
			this->aspectRatio = aspectRatio; 
			setProjectionMatrix(glm::perspective(fovy, aspectRatio, near, far));
		}

		inline void setPosition(glm::vec3 position) { this->position = position; }
		inline const glm::vec3& getPosition() const { return position; }

		inline void setLookAtDirection(glm::vec3 lookAtDirection) 
		{
			this->lookAtDirection = lookAtDirection;
		}
	
		inline const glm::vec3& getLookAtDirection() const { return lookAtDirection; }

		inline const glm::mat4& getViewMatrix() const { return viewMatrix; }
		glm::mat4 getViewProjectionMatrix() { return getProjectionMatrix() * viewMatrix; }

		void updateViewMatrix();

	private:

		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseMoved(MouseMovedEvent& e);
		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);

		float aspectRatio;
		float fovy;
		float near;
		float far;

		glm::vec3 position;
		glm::vec3 lookAtDirection;
		glm::vec3 upDirection;

		glm::mat4 viewMatrix;

		float cameraPositionSpeed;

		bool mousePressed;
		bool ctrlPressed;
		std::tuple<double, double> mousePos;
	};
	
	

}
#endif // __EDITORCAMERA_H__