#include "rigidbody.hpp"
#include "bvh.hpp"
#include "core/assert.hpp"

namespace Physicc
{
	namespace Broadphase
	{

		struct PotentialContact
		{
			PotentialContact(RigidBody& body1, RigidBody& body2) //TODO: Add a copy constructor to RigidBody in Development
			: rb1(body1), rb2(body2)
			{

			}
            RigidBody rb1;
			RigidBody rb2;  
		};

		
		//Anonymous namespace to make the implementation of the function invisible
		//outside this source file
		namespace
		{

			//inlined function to improve readability
			[[nodiscard]] inline bool isLeaf(BVHNode* node)
			{
				return (node->left == nullptr) && (node->right == nullptr);
			}

			void getPotentialContactsWith(BVHNode* node1, BVHNode* node2, std::vector<PotentialContact>& collisionArray) //TODO: Figure out a name for the nodes here
			{ //TODO: Discuss if there is any advantage to inlining this (i.e. using the `inline` keyword.)
				//TODO: Check for the nullity of leaf nodes, and make sure that the function acts accordingly.
				if (isLeaf(node1) && isLeaf(node2))
				{
					//If both nodes are leaves, then we have a PotentialContact, and we add the collision pair to the collisionArray
					collisionArray.push_back(PotentialContact(*(node1->body), *(node2->body)));
				} else if (isLeaf(node1) && !isLeaf(node2))
				{
					//If only of the nodes is a leaf, then make sure that node2 is the leaf node and that we recurse through node1
					getPotentialContactsWith(node2, node1, collisionArray);
					//TODO: Discuss and figure out if the (minor?) performance penalty is worth it, or if we should just go for the following
					/*
					 * getPotentialContactsWith(node1, node2->left, collisionArray);
					 * getPotentialContactsWith(node1, node2->right, collisionArray);
					 */
				} else if (!isLeaf(node1) && isLeaf(node2))
				{
					//recurse through both the left and right child of the non-leaf node (i.e. node1)
					getPotentialContactsWith(node1->left, node2, collisionArray);
					getPotentialContactsWith(node1->right, node2, collisionArray);
				} else
				{
					/** Both nodes are non-leaf nodes */

					//Recurse through node1, with node2 constant.
					getPotentialContactsWith(node1->left, node2, collisionArray);
					getPotentialContactsWith(node1->right, node2, collisionArray);

					//Recurse through node2, with node1 constant.
					getPotentialContactsWith(node1, node2->left, collisionArray);
					getPotentialContactsWith(node1, node2->right, collisionArray);

				}
			}

			void getPotentialContacts(BVHNode* node, std::vector<PotentialContact>& collisionArray) //TODO:Figure out a name for this array
			{ //TODO: Discuss if there is any advantage to inlining this (i.e. using the `inline` keyword.)
				// LIGHT_

				if (isLeaf(node))
					return;

				getPotentialContacts(node->left, collisionArray);
				getPotentialContacts(node->right, collisionArray);

				if ((node->left)->volume.overlapsWith((node->right)->volume))
				{
					getPotentialContactsWith(node->left, node->right, collisionArray);
				}
			}
		}
		
	}
}
