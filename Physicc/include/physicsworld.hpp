#ifndef __PHYSICC_H__
#define __PHYSICC_H__
#include "../../Light/libs/glm/glm/glm.hpp"
#include"rigidbody.hpp"
#include<vector>

class PhysicsWorld
{
    public:
    PhysicsWorld(glm::vec3 gravity );
    void setgravity(glm::vec3);
    inline const glm::vec3 getgravity() const{
        return m_gravity;
    }

    void addrigidbody(RigidBody object);
    void stepsimulation(float time);
    
    
    
    
    private:
    glm::vec3 m_gravity;
    std:: vector<RigidBody> m_objects;
    

    
};






#endif // __PHYSICC_H__