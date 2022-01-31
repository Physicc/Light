#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "tools/Tracy.hpp"

#include "glm/glm.hpp"
#include "collider.hpp"
#include <memory>

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
			RigidBody(const RigidBody& other) = default;

			[[nodiscard]] inline glm::vec3 getVelocity() const
			{
				ZoneScoped;

				return m_velocity;
			}

			inline void setVelocity(const glm::vec3& velocity)
			{
				ZoneScoped;

				m_velocity = velocity;
			}

			inline void setGravityScale(const float gravityScale)
			{
				ZoneScoped;

				m_gravityScale = gravityScale;

			}

			inline void setPosition(const glm::vec3& position)
			{
				m_collider->setPosition(position);
			}

			void setForce();

			[[nodiscard]] inline BoundingVolume::AABB getAABB() const
			{
				return m_collider->getAABB();
			}

			[[nodiscard]] inline glm::vec3 getCentroid() const
			{
				return m_collider->getCentroid();
			}

			inline void setCollider(const std::shared_ptr<Collider> collider)
			{
				m_collider = collider;
			}

		private:
			glm::vec3 m_force;
			std::shared_ptr<Collider> m_collider;
			float m_mass;
			glm::vec3 m_velocity;
			float m_gravityScale;

			friend class PhysicsWorld;
			//PhysicsWorld needs to have access to all of RigidBody's private
			//members for functions like stepSimulation, etc.
	};
}

#endif // __RIGIDBODY_H__
