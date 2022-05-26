#ifndef __EDITORCAMERA_H__
#define __EDITORCAMERA_H__

#include "core/base.hpp"

#include "light/rendering/camera.hpp"
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

		void setViewportSize(uint32_t width, uint32_t height);

		void blockUpdate(bool block) { m_blockUpdate = block; }

		const glm::mat4& getViewMatrix() const { return m_viewMatrix; }
		void setViewMatrix(glm::mat4 viewMatrix) { m_viewMatrix = viewMatrix; recalculateOrientation(); }
		glm::mat4 getViewProjectionMatrix() { return getProjectionMatrix() * m_viewMatrix; }

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		glm::quat getOrientation() const;
		float getDistance() const { return m_distance; }


	private:

		bool onMouseScrolled(MouseScrolledEvent& e);

		void updateView();
		void updateProjection();
		void recalculateOrientation();

		glm::vec3 calculatePosition() const;

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

		float m_aspectRatio;
		float m_fovy;
		float m_near;
		float m_far;

		glm::mat4 m_viewMatrix;
		glm::vec3 m_position = {-0.754f, 0.651f, 1.758f};
		glm::vec3 m_focalPoint = {0.0f, 0.0f, 0.0f};

		float m_distance = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;

		glm::vec2 m_initialMousePos = {0.0, 0.0};

		uint32_t m_viewportWidth = 1280, m_viewportHeight = 720;

		bool m_blockUpdate = false;
	};



}
#endif // __EDITORCAMERA_H__
