#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "tools/Tracy.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "boundingvolume.hpp"
#include <vector>

namespace Physicc
{
	/**
	 * @brief Collider class
	 *  
	 * This is a virtual class which acts as the base for all the shape specific classes
	 */
	class Collider
	{
		public:
			Collider(glm::vec3 position = glm::vec3(0),
			         glm::vec3 rotation = glm::vec3(0),
			         glm::vec3 scale = glm::vec3(1));

			/**
	 		 * @brief Get Position of object's center
	 		 *
	 		 * @return glm::vec3
	 		 */
			[[nodiscard]] inline glm::vec3 getPosition()
			{
				ZoneScoped;

				return m_position;
			}

			/**
			 * @brief get Angle of rotation of the object about its center
			 *
			 * @return glm::vec3
			 */
			[[nodiscard]] inline glm::vec3 getRotate()
			{
				ZoneScoped;

				return m_rotate;
			}

			/**
			 * @brief get Scale of object
			 *
			 * @return glm::vec3
			 */
			[[nodiscard]] inline glm::vec3 getScale()
			{
				ZoneScoped;

				return m_scale;
			}

			/**
			 * @brief get Transform matrix of object
			 *
			 * @return glm::mat4
			 */
			[[nodiscard]] inline glm::mat4 getTransform()
			{
				ZoneScoped;

				return m_transform;
			}

			/**
			 * @brief set Position of object's center
			 *
			 * @param position Takes the (x,y,z) coordinates to place the object's center at
			 */
			inline void setPosition(glm::vec3 position)
			{
				ZoneScoped;

				m_position = position;
			}

			/**
			 * @brief Set rotation of object about it's center
			 *
			 * @param rotate vec3 containing rotation values about x, y, z axes
			 */
			inline void setRotate(glm::vec3 rotate)
			{
				ZoneScoped;

				m_rotate = rotate;
			}

			/**
			 * @brief get Position of object's center
			 *
			 * @param scale New scale of the object
			 */
			inline void setScale(glm::vec3 scale)
			{
				ZoneScoped;

				m_scale = scale;
			}

			void updateTransform();

			virtual BoundingVolume::AABB getAABB() const = 0;
			//Each child will calculate its AABB according to its own shape

		protected:
			enum Type
			{
				e_box = 0,
				e_sphere = 1,
				e_typecount = 2
			};

			glm::vec3 m_position;
			glm::vec3 m_rotate;
			glm::vec3 m_scale;
			glm::mat4 m_transform;
			Type m_objectType;
	};

	/** 
	 * @brief BoxCollider class
	 *  
	 * Box shaped collider, holds the shape and transform of the body.
	 */
	class BoxCollider : public Collider
	{
		public:
			BoxCollider(glm::vec3 position = glm::vec3(0),
			            glm::vec3 rotation = glm::vec3(0),
			            glm::vec3 scale = glm::vec3(1));

			[[nodiscard]] BoundingVolume::AABB getAABB() const override;

		private:
			std::vector<glm::vec4> m_vertices;
	};

	/** 
	 * @brief SphereCollider class
	 *  
	 * Sphere shaped collider, holds the radius and transform of the body
	 */
	class SphereCollider : public Collider
	{
		public:
			SphereCollider(float radius = 1.0f,
			               glm::vec3 position = glm::vec3(0),
			               glm::vec3 rotation = glm::vec3(0),
			               glm::vec3 scale = glm::vec3(1));

			[[nodiscard]] BoundingVolume::AABB getAABB() const override;

		private:
			float m_radius;
	};
}

#endif // __COLLIDER_H__
