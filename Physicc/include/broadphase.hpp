#include "rigidbody.hpp"
#include "bvh.hpp"
#include "core/assert.hpp"

namespace Physicc
{
    namespace Broadphase
    {
        struct PotentialContact
		{
            /**
	        * @brief A struct that's used to store a pair of rigidbody
	        * that might be colliding.
	        */
			PotentialContact(RigidBody& body1, RigidBody& body2)
            : rb1(body1), rb2(body2)
		    {
		    }
            RigidBody rb1, rb2;
		};

		std::vector<PotentialContact> getPotentialContacts(BVHNode* node);
    }

}