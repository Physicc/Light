#include "rigidbody.hpp"

RigidBody::RigidBody(glm::vec3 linearPosition,glm::vec3 linearVelocity)
    :m_linearPosition (linearPosition),
     m_linearVelocity (linearVelocity)
    {}

void RigidBody::updatePosition(float time)
{
    m_linearPosition += m_linearVelocity * time;
}


