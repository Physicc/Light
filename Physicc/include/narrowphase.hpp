#ifndef __NARROWPHASE_H__
#define __NARROWPHASE_H__

#include "glm/glm.hpp"
#include "core/assert.hpp"
#include "rigidbody.hpp"
#include "broadphase.hpp"
#include <vector>
#include <array>

/**
 * TODO: Write BoxSphere and BoxBox functions, make it so that the functions return all the
 * information required for contact resolution.
 * 
 */

namespace Physicc::Narrowphase
{
	struct Contact
	{

		Contact(glm::vec3 cP, glm::vec3 cN, double p, bool f = false) 
		: contactPoint(cP), contactNormal(cN), penetration(p)
		{
		}

		glm::vec3 contactPoint;
		glm::vec3 contactNormal;
		double penetration;
		// Friction has been removed as it is a property of rigid body and not of Contact
	};

	struct collisionFunctionMatrix; // forward declaration of the struct

	class CollisionDetector
	{
		public:
		CollisionDetector(std::vector<Broadphase::PotentialContact> &v)
		: collisionArray(v)
		{
		}
		

		// Move individual components to individual function

		bool generateContacts()
		{
			// Take in the entire array of potential contacts and then
			// traverse through every single one, and dispatch it to the
			// correct narrow phase collision detecting function.
		}

		std::vector<Narrowphase::Contact> getContacts()
		{
		}

		private:
		std::vector<Broadphase::PotentialContact> collisionArray;
		std::vector<Narrowphase::Contact> collisionInfo;
		// the above should return a complete list of contact info for every collision
		std::vector<std::pair<RigidBody, RigidBody>>> collision;
		collisionFunctionMatrix m;
	};

	template <typename FirstBody, typename SecondBody>
	bool checkCollision(Broadphase::PotentialContact a) {
		LIGHT_ASSERT(true, "Collision checking for the given types is not supported yet. \
						   Please file a bug report if you can reproduce this issue.")
	}

	// This file includes specific overloads of the above function.
	#include "narrowphase.ipp"

	// The collisionFunctionMatrix is defined here because it needs to be able to see the full
	// definitions of all the functions before it can do its thing
	template <typename... CollisionTypes>
	struct collisionFunctionMatrix
	{
		collisionFunctionMatrix()
		{
			// When an object of this struct is instatiated, we will contruct the matrix.

		}

		constexpr std::array<bool (*)(Broadphase::PotentialContact), sizeof...(CollisionTypes)> matrix;

		private:
		// We need a bunch of helper functions to construct the matrix
	};
}

#endif //__NARROWPHASE_H__
