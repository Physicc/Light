#ifndef __BROADPHASE_HPP__
#define __BROADPHASE_HPP__

#include "rigidbody.hpp"
#include "bvh.hpp"
#include "core/assert.hpp"

namespace Physicc::Broadphase
{
	struct PotentialContact
	{
		/**
		* @brief A struct that's used to store a pair of RigidBodies
		* that might be colliding.
		*/
		PotentialContact(RigidBody* body1, RigidBody* body2)
		: rb1(body1), rb2(body2)
		{
		}
		RigidBody* rb1, rb2;
	};

	std::vector<PotentialContact> getPotentialContacts(BVHNode* node);
}

#endif // __BROADPHASE_HPP__
