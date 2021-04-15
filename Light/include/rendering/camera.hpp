#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "core/base.hpp"

namespace Light
{
	class Camera
	{
	public:
		Camera(glm::mat4 projectionMatrix = glm::mat4(1.0f))
			:	projectionMatrix(projectionMatrix)
		{}

		virtual ~Camera() = default;

		inline const glm::mat4& getProjectionMatrix() { return projectionMatrix; }
		inline void setProjectionMatrix(glm::mat4 projectionMatrix) { this->projectionMatrix = projectionMatrix; }

	protected:
		glm::mat4 projectionMatrix;
	};

}

#endif // __CAMERA_H__