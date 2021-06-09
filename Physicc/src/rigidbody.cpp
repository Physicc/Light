#include "rigidbody.hpp"

RigidBody::RigidBody(glm::vec3 linearposition,glm::vec3 linearvelocity)

    :m_linearposition(linearposition),
     m_linearvelocity(linearvelocity)
    
    { 
    }
void RigidBody::setlinearposition(glm::vec3 position){
    m_linearposition=position;
}

void RigidBody::setlinearvelocity(glm::vec3 velocity){
    m_linearvelocity=velocity;
}


