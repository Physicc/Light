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
	 * @brief RigidBody initialized with a mass velocity, and a float storing
	 * the scale of the gravity is acting on the object.
	 */
	RigidBody::RigidBody(const float mass, const glm::vec3& velocity,
						const float gravityScale = 1)
		:	m_force(glm::vec3(0)),
			m_mass(mass),
			m_velocity(velocity),
			m_gravityScale(gravityScale)
	{
	}
}
