#ifndef __PHYSICC_H__
#define __PHYSICC_H__
#include "../../Light/libs/glm/glm/glm.hpp"

class PhysicsWorld
{
    public:
    PhysicsWorld(glm::vec3 gravity = glm::vec3(0,0,0));
    void setgravity(glm::vec3);
    glm::vec3 getgravity() const;
    
    
    
    
    
    private:
    glm::vec3 m_gravity;
    

    
};






#endif // __PHYSICC_H__