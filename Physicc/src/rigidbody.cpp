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
RigidBody::RigidBody(const glm::vec3& linearPosition, const glm::vec3& linearVelocity)
    : m_linearPosition(linearPosition),
      m_linearVelocity(linearVelocity)
    {}

/** @fn void RigidBody::updatePosition(float timestep)
 *  @brief updates the position  
 *  @param timestep: input, float type,time interval
 */
void RigidBody::updatePosition(float timestep)
{
    m_linearPosition += m_linearVelocity * timestep;
}


