#include "rendering/camera.hpp"

namespace Light
{
	void Camera::recalculateViewMatrix() 
	{
		viewMatrix = glm::lookAt(position, position + lookAtDirection, upDirection);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far) 
		: Camera(glm::ortho(left, right, bottom, top, near, far), glm::vec3(0.0f, 0.0f, 0.0f))
	{
		recalculateViewMatrix();
	}
	
	OrthographicCamera::~OrthographicCamera() 
	{

	}

	PerspectiveCamera::PerspectiveCamera(float fovy, float aspectRatio, float near, float far)
		: Camera(glm::perspective(glm::radians(fovy), aspectRatio, near, far), glm::vec3(0.0f, 0.0f, 1.0f))
	{
		
	}
	
	PerspectiveCamera::~PerspectiveCamera() 
	{
		
	}
}