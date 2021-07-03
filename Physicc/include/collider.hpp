#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

			inline glm::vec3 getPos();
			inline glm::vec3 getRotate();
			inline glm::vec3 getScale();
			inline glm::mat4 getTransform();

			inline void setPos(glm::vec3 newpos);
			inline void setRotate(glm::vec3 newrotate);
			inline void setScale(glm::vec3 newscale);

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
			BoxCollider(glm::vec3 size = glm::vec3(1),
			            glm::vec3 position = glm::vec3(0),
			            glm::vec3 rotation = glm::vec3(0),
			            glm::vec3 scale = glm::vec3(1));

			~BoxCollider() = default;

			AABB getAABB() const override;

		private:
			glm::vec3 m_size;
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