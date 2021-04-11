#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/base.hpp"

namespace Light
{
	class Camera
	{
	public:
		Camera(glm::mat4 projectionMatrix, glm::vec3 position)
			:	projectionMatrix(projectionMatrix), 
				position(position),
				lookAtDirection(0.0f, 0.0f, -1.0f),
				upDirection(0.0f, 1.0f, 0.0f)
		{

		}

		virtual ~Camera() = default;

		const glm::vec3& getPosition() { return position; }
		const glm::vec3& getLookAtDirection() { return lookAtDirection; }
		const glm::vec3& getUpDirection() { return upDirection; }
		void setPosition(glm::vec3 position) { this->position = position; recalculateViewMatrix(); }
		void setLookAtDirection(glm::vec3 lookAtDirection) { this->lookAtDirection = lookAtDirection; recalculateViewMatrix(); }
		void setUpDirection(glm::vec3 upDirection) { this->upDirection = upDirection, recalculateViewMatrix(); }
		const glm::mat4& getViewMatrix() { return viewMatrix; }
		const glm::mat4& getProjectionMatrix() { return projectionMatrix; }
		const glm::mat4& getViewProjectionMatrix() { return viewProjectionMatrix; }

	protected:
		glm::mat4 viewMatrix;
		glm::mat4 viewProjectionMatrix;
		glm::mat4 projectionMatrix;

		glm::vec3 position;
		glm::vec3 lookAtDirection;
		glm::vec3 upDirection;

		void recalculateViewMatrix();

	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float near, float far);
		~OrthographicCamera();
	};
	
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fovy, float aspectRatio, float near, float far);

		void setProjection(float fovy, float aspectRatio, float near, float far);

		~PerspectiveCamera();

	};

}

#endif // __CAMERA_H__