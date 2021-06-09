#include "physicsworld.hpp"


PhysicsWorld::PhysicsWorld(glm::vec3 gravity)
    :m_gravity(gravity)
    
    { 
    }

void PhysicsWorld::setgravity(glm::vec3 gravity){
    m_gravity=gravity;
}

void PhysicsWorld::addrigidbody(RigidBody object){
    m_objects.push_back(object);
}

void PhysicsWorld::stepsimulation(float time){
    for(int i=0;i<m_objects.size();i++){
        m_objects[i].updateposition(time);
    }
}
