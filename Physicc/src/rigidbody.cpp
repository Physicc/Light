#include "rigidbody.hpp"

RigidBody::RigidBody(const glm::vec3& linearPosition, const glm::vec3& linearVelocity)
    : m_linearPosition(linearPosition),
     m_linearVelocity(linearVelocity)
    {}

void RigidBody::updatePosition(float time)
{
    m_linearPosition += m_linearVelocity * time;
}


