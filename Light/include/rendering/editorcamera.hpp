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

		void setViewportSize(int width, int height);

		inline const glm::mat4& getViewMatrix() const { return viewMatrix; }
		glm::mat4 getViewProjectionMatrix() { return getProjectionMatrix() * viewMatrix; }

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		glm::quat getOrientation() const;

	private:

		bool onMouseScrolled(MouseScrolledEvent& e);

		void updateView();
		void updateProjection();

		glm::vec3 calculatePosition() const;

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

		float aspectRatio;
		float fovy;
		float near;
		float far;

		glm::mat4 viewMatrix;
		glm::vec3 position = {-0.754f, 0.651f, 1.758f};
		glm::vec3 focalPoint = {0.0f, 0.0f, 0.0f};

		float distance = 10.0f;
		float pitch = 0.0f, yaw = 0.0f;

		glm::vec2 initialMousePos = {0.0, 0.0};

		float viewportWidth = 1280, viewportHeight = 720;
	};
	
	

}
#endif // __EDITORCAMERA_H__