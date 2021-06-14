/** @file rigidbody.cpp
 *  @brief Defines a Rigid Body.
 *  @author Divyansh Tiwari (divyanshtiwari237)
 *  @author Neilabh Banzal (Neilabh21)
 *  @author Tirthankar mazumder (wermos)
 *  @bug No known bugs.
 */

/* -- Includes -- */
/*rigidbody header*/

#include "rigidbody.hpp"

/** @brief RigidBody initialized with velocity.
 */
RigidBody::RigidBody(const float mass, const glm::vec3& velocity,
                     const bool isGravity)
	: m_mass(mass),
	  m_velocity(velocity),
	  m_isGravity(isGravity),
	  m_force(glm::vec3(0, 0, 0))
{}
