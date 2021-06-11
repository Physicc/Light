#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include "../../Light/libs/glm/glm/glm.hpp"

/** @brief Rigid Body Class
 *
 *  This class describes and propagates the properties of each Rigid Body.
 */
class RigidBody
{
    public:
        RigidBody(const glm::vec3& velocity);

        inline glm::vec3 getLinearVelocity() const
        {
            return m_velocity;
        }

        inline void setLinearVelocity(const glm::vec3& velocity)
        {
            m_velocity = velocity;
        }

    private:
        glm::vec3 m_velocity;
};

#endif // __RIGIDBODY_H__
