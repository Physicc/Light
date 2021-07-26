#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

namespace Physicc
{
	/**
	 * @brief Axis Aligned Bounding Box 
	 * 
	 * Helper class to store diagonally opposite points of the AABB
	 * 
	 */
	struct AABB
	{
		glm::vec3 lowerBound;
		glm::vec3 upperBound;
	};

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
			virtual ~Collider() = 0;        //Make pure virtual class so no instantiation possible

			/**
	 		 * @brief Get Position of object's center
	 		 *
	 		 * @return glm::vec3
	 		 */
			[[nodiscard]] inline glm::vec3 getPosition()
			{
				return m_position;
			}

			/**
			 * @brief get Angle of rotation of the object about it's center
			 *
			 * @return glm::vec3
			 */
			[[nodiscard]] inline glm::vec3 getRotate()
			{
				return m_rotate;
			}

			/**
			 * @brief get Scale of object
			 *
			 * @return glm::vec3
			 */
			[[nodiscard]] inline glm::vec3 getScale()
			{
				return m_scale;
			}

			/**
			 * @brief get Transform matrix of object
			 *
			 * @return glm::mat4
			 */
			[[nodiscard]] inline glm::mat4 getTransform()
			{
				return m_transform;
			}

			/**
			 * @brief set Position of object's center
			 *
			 * @param newpos Takes the (x,y,z) coordinates to place the object at
			 */
			inline void setPosition(glm::vec3 position)
			{
				m_position = position;
			}

			/**
			 * @brief Set rotation of object about it's center
			 *
			 * @param newrotate vec3 containing rotation values about x,y,z axes
			 */
			inline void setRotate(glm::vec3 rotate)
			{
				m_rotate = rotate;
			}

			/**
			 * @brief get Position of object's center
			 *
			 * @param newscale New scale of the object
			 */
			inline void setScale(glm::vec3 scale)
			{
				m_scale = scale;
			}

			void updateTransform();

			virtual AABB getAABB() const;    //Each child will calculate AABB according to it's own shape

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

			~BoxCollider() = default;

			AABB getAABB() const override;

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

			~SphereCollider() = default;

			AABB getAABB() const override;

		private:
			float m_radius;
	};
}

#endif // __COLLIDER_H__