#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "glm/glm.hpp"
#include "collider.hpp"

namespace Physicc
{
	/**
	 * @brief Rigid Body Class
	 *
	 * This class describes and propagates the properties of each Rigid Body.
	 */
	class RigidBody
	{
		public:
			RigidBody(float mass, const glm::vec3& velocity, float gravityScale);

			[[nodiscard]] inline glm::vec3 getVelocity() const
			{
				return m_velocity;
			}

			
            inline void setVelocity(const glm::vec3& velocity)
			{
				m_velocity = velocity;
			}

			inline void setGravityScale(const float gravityScale)
			{
				m_gravityScale = gravityScale;

			}

			void setForce();

			[[nodiscard]] inline BoundingVolume::AABB getAABB() const
			{
				return m_collider.getAABB();
			}
		private:
			glm::vec3 m_force;
			BoxCollider m_collider;
			float m_mass;
			glm::vec3 m_velocity;
			float m_gravityScale;

			friend class PhysicsWorld;
			//PhysicsWorld needs to have access to all of RigidBody's private
			//members for functions like stepSimulation, etc.
	};
}

#endif // __RIGIDBODY_H__
