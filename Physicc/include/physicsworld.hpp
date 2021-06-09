#ifndef __PHYSICC_H__
#define __PHYSICC_H__
#include "../../Light/libs/glm/glm/glm.hpp"

class PhysicsWorld
{
    public:
    PhysicsWorld(glm::vec3 gravity );
    void setgravity(glm::vec3);
    inline const glm::vec3 getgravity() const{
        return m_gravity;
    }
    
    
    
    
    
    private:
    glm::vec3 m_gravity;
    

    
};






#endif // __PHYSICC_H__