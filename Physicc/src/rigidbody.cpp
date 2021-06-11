/** @file rigidbody.cpp
 *  @brief Defines a Rigid Body.
 *  @author Divyansh Tiwari (divyanshtiwari237)
 *  @author Neilabh Banzal (Neilabh21)
 *  @bug No known bugs.
 */

/* -- Includes -- */
/*rigidbody header*/

#include "rigidbody.hpp"

/** @brief RigidBody initialized with linear Position and Velocity.
 */
RigidBody::RigidBody(const glm::vec3& velocity)
    : m_velocity(velocity)
    {}
