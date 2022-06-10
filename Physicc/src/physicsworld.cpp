/**
 * @file physicsworld.cpp
 * @brief The Physics World.
 *
 * The Physics World file class conatins the Physics Model for the Physics
 * Engine. This includes the Gravity Model, Rigid Body objects.
 *
 * @author Divyansh Tiwari (divyanshtiwari237)
 * @author Neilabh Banzal (Neilabh21)
 * @author Tirthankar Mazumder (wermos)
 * @bug No known bugs.
 */

/* -- Includes -- */
/* physicsworld header */

#include "tools/Tracy.hpp"

#include "physicsworld.hpp"

namespace Physicc
{
	/**
	 * @brief Physics World initialisation with gravity.
	 *
	 * This initialises the Physics World with gravity, input from the ---?---.
	 */
	PhysicsWorld::PhysicsWorld(const glm::vec3& gravity)
		: m_gravity(gravity)    
	{
	}

	/**
	 * @brief Add a new RigidBody to m_objects
	 * @param object: input, const RigidBody& type
	 */
	void PhysicsWorld::addRigidBody(const RigidBody& object)
	{
		ZoneScoped;

		m_objects.push_back(object);
	}

	/**
	 * @brief steps the simulation by time timestep
	 * @param timestep: input, float type, time interval
	 */
	void PhysicsWorld::stepSimulation([[maybe_unused]] float timestep)
	{
		ZoneScoped;

		for(std::size_t i = 0; i < m_objects.size(); i++)
		{
		}
	}
}

