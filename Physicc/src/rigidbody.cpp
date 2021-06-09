#include "rigidbody.hpp"

RigidBody::RigidBody(glm::vec3 linearposition,glm::vec3 linearvelocity)

    :m_linearPosition(linearposition),
     m_linearVelocity(linearvelocity)
    
    { 
    }
void RigidBody::updatePosition(float time){
    m_linearPosition+=m_linearVelocity*time;
}


