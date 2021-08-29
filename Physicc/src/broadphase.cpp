#include "rigidbody.hpp"
#include "bvh.hpp"

namespace Physicc
{

	namespace Broadphase
	{
		struct PotentialContact
		{
			RigidBody bodies[2]; //TODO : Figure out a name for this array
		};

		[[nodiscard]] inline bool isLeaf(BVHNode* node)
		{
			return (node->left == nullptr) && (node->right == nullptr);
		}


		void getPotentialContacts(BVHNode* node, std::vector<PotentialContact>& collisionArray) //TODO:Figure out a name for this array
		{

			if (isLeaf(node))
				return;

			getPotentialContacts(node->left, collisionArray);
			getPotentialContacts(node->right, collisionArray);

            if ((node->left)->volume.overlapsWith((node->right)->volume))
			{
				getPotentialContactsWith(node->left, node->right, collisionArray);
			}
		}

		void getPotentialContactsWith(BVHNode* node1, BVHNode* node2, std::vector<PotentialContact>& collisionArray) //TODO: Figure out a name for the nodes here
		{
			


		}
	}
}