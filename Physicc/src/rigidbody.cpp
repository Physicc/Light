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
RigidBody::RigidBody(const glm::vec3& velocity)
    : m_velocity(velocity)
    {}
