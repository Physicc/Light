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
			RigidBody(float mass, const glm::vec3& velocity, bool isGravity);

			[[nodiscard]] inline glm::vec3 getVelocity() const
			{
				return m_velocity;
			}

			inline void setVelocity(const glm::vec3& velocity)
			{
				m_velocity = velocity;
			}

			[[nodiscard]] inline bool isGravity() const
			{
				//Is gravity acting on this object?
				return m_isGravity;
			}

			void setForce();

		private:
			glm::vec3 m_force{};
			BoxCollider m_collider{};
			float m_mass;
			glm::vec3 m_velocity;
			bool m_isGravity;			//Probably change this from bool isGravity to float Gravity. 
										//Reference: http://www.r-5.org/files/books/computers/algo-list/realtime-3d/Ian_Millington-Game_Physics_Engine_Development-EN.pdf 
										//Pg. 50, The value of g
			friend class PhysicsWorld;
			//PhysicsWorld needs to have access to all of RigidBody's private
			//members for functions like stepSimulation, etc.
	};
}

#endif // __RIGIDBODY_H__
