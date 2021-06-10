#include "physicsworld.hpp"


PhysicsWorld::PhysicsWorld(const glm::vec3& gravity)
    : m_gravity(gravity)    
    {}

void PhysicsWorld::addRigidBody(const RigidBody& object)
{
    m_objects.push_back(object);
}

void PhysicsWorld::stepSimulation(float time)
{
    for(int i = 0; i < m_objects.size(); i++)
    {
        m_objects[i].updatePosition(time);
    }
}
