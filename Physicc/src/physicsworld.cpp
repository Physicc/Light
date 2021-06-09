#include "physicsworld.hpp"


PhysicsWorld::PhysicsWorld(glm::vec3 gravity)
    :m_gravity(gravity)
    {

    }

void PhysicsWorld::setgravity(glm::vec3 gravity){
    m_gravity=gravity;
}

glm::vec3 PhysicsWorld::getgravity(){
    return m_gravity;
}