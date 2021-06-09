#ifndef __PHYSICC_H__
#define __PHYSICC_H__

#include "../../Light/libs/glm/glm/glm.hpp"
#include "rigidbody.hpp"
#include <vector>

class PhysicsWorld
{
    public:
    PhysicsWorld(glm::vec3 gravity);
    
    inline void setGravity(glm::vec3 gravity)
    {
        m_gravity=gravity;
    }

    inline const glm::vec3 getGravity() const
    {
        return m_gravity;
    }

    void addRigidBody(RigidBody object);
    void stepSimulation(float time);
    
    
    
    
    private:
    glm::vec3 m_gravity;
    std:: vector<RigidBody> m_objects;
    

    
};






#endif // __PHYSICC_H__