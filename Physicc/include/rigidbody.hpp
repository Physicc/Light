#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__
#include "../../Light/libs/glm/glm/glm.hpp"
class RigidBody
{
    public:
    RigidBody(glm::vec3 linearposition,glm::vec3 linearvelocity);

    inline const glm::vec3 getlinearposition() const{
        return m_linearposition;
    }
    inline const glm::vec3 getlinearvelocity() const{
        return m_linearvelocity;
    }
    void setlinearposition(glm::vec3);
    void setlinearvelocity(glm::vec3);

    void updateposition(float time);

    private:
    glm::vec3 m_linearposition;
    glm::vec3 m_linearvelocity;

};


#endif // __RIGIDBODY_H__