/**
 * @file rigidbody.cpp
 * @brief Defines a Rigid Body.
 * @author Divyansh Tiwari (divyanshtiwari237)
 * @author Neilabh Banzal (Neilabh21)
 * @author Tirthankar mazumder (wermos)
 * @bug No known bugs.
 */

/* -- Includes -- */
/*rigidbody header*/

#include "rigidbody.hpp"

namespace Physicc
{
	/**
	 * @brief RigidBody initialized with a mass velocity, and a bool storing whether
	 * gravity is acting on the object or not.
	 */
	RigidBody::RigidBody(const float mass, const glm::vec3& velocity,
						const float gravityScale = 1)
		:	m_force(glm::vec3(0)),
			m_mass(mass),
			m_velocity(velocity),
			m_gravityScale(gravityScale)
	{
	}

	RigidBody::RigidBody(const RigidBody& other)
		:	m_force(other.m_force),
			m_collider(other.m_collider),
			m_mass(other.m_mass),
			m_velocity(other.m_velocity),
			m_gravityScale(other.m_gravityScale)
	{
	}
}
