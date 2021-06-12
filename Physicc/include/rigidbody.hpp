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

        inline glm::vec3 getVelocity() const
        {
            return m_velocity;
        }

        inline void setLinearVelocity(const glm::vec3& velocity)
        {
            m_velocity = velocity;
        }

	inline bool isGravity()
    {
		//Is gravity acting on this object?
		return m_isGravity;
	}

    private:
    	glm::vec3 m_force;
	    //TODO: Insert Collider here
	    float m_mass;
        glm::vec3 m_velocity;
    	bool isGravity;

	friend class PhysicsWorld;
	//PhysicsWorld needs to have access to all of RigidBody's private
	//members for functions like stepSimulation, etc.
};

#endif // __RIGIDBODY_H__
