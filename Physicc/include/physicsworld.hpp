#ifndef __PHYSICC_H__
#define __PHYSICC_H__

#include "glm/glm.hpp"
#include "rigidbody.hpp"
#include <vector>

namespace Physicc
{
	/**
	 * @brief World's Physics Class
	 *
	 * This class describes and propagates the properties of each object using the
	 * Physics Model.
	 */
	class PhysicsWorld
	{
		public:
			PhysicsWorld(const glm::vec3& gravity);

			inline void setGravity(const glm::vec3& gravity)
			{
				m_gravity = gravity;
			}

			[[nodiscard]] inline glm::vec3 getGravity() const
			{
				return m_gravity;
			}

			void addRigidBody(const RigidBody& object);
			void stepSimulation(float timestep);

		private:
			glm::vec3 m_gravity;
			std::vector<RigidBody> m_objects;
	};
}

#endif // __PHYSICC_H__

