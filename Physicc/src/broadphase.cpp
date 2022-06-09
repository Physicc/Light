/**
 * @file broadphase.cpp
 * @brief The Broadphase implementation.
 *
 * The Broadphase file consists of methods which implement the broadphase
 * collision-detection algorithm using the bvh node defined in bvh.hpp
 *
 * @author Tirthankar Mazumder (wermos)
 * @author Divyansh Tiwari (divyanshtiwari237)
 * @bug No known bugs.
 */

/* -- Includes -- */
/* broadphase header */

#include "broadphase.hpp"

namespace Physicc::Broadphase
{
	//Anonymous namespace to make the implementation of the function invisible
	//outside this source file
	namespace BroadphaseImpl
	{
		//inlined function to improve readability
		/**	 		
		* @brief Inlined function used to check whether the given node is a leaf node
		* @param node: BVHNode* node type
		*/
		[[nodiscard]] inline bool isLeaf(BVHNode* node)
		{
			return (node->left == nullptr) && (node->right == nullptr);
		}

		/**
		* @brief Function that calculates and stores potential collision pairs between two the nodes provided
		* @param node1: BVHNode* node type 
		* @param node2: BVHNode* node type
		* @param collisionArray: std::vector<PotentialContact>& type
		*/
		void getPotentialContactsWith(BVHNode* node1, BVHNode* node2, std::vector<PotentialContact>& collisionArray)
		{
			if (!(*node1).overlapsWith(*node2))
			{
				// if the nodes don't overlap, then no need to check for collisions
				return;
			}else if (isLeaf(node1) && isLeaf(node2))
			{
				//If both nodes are leaves, then we have a PotentialContact, and we add the collision pair to the collisionArray
				collisionArray.push_back(PotentialContact(node1->body, node2->body));
			} else if (isLeaf(node1) && !isLeaf(node2))
			{
				//If only one of the nodes is a leaf, then make sure that node2 is the leaf node and that we recurse through node1
				getPotentialContactsWith(node2, node1, collisionArray);
				//TODO: #42 @dropTableUsers42 Discuss and figure out if the (minor?) performance penalty is worth it, or if we should just go for the following
				/*
				 * getPotentialContactsWith(node1, node2->left, collisionArray);
				 * getPotentialContactsWith(node1, node2->right, collisionArray);
				 */
			} else if (!isLeaf(node1) && isLeaf(node2))
			{
				//recurse through both the left and right child of the non-leaf node (i.e. node1)
				getPotentialContactsWith(node1->left.get(), node2, collisionArray);
				getPotentialContactsWith(node1->right.get(), node2, collisionArray);
			} else
			{
				/** Both nodes are non-leaf nodes */

				//Recurse through node1, with node2 constant.
				getPotentialContactsWith(node1->left.get(), node2->left.get(), collisionArray);
				getPotentialContactsWith(node1->left.get(), node2->right.get(), collisionArray);

				//Recurse through node2, with node1 constant.
				getPotentialContactsWith(node1->right.get(), node2->left.get(), collisionArray);
				getPotentialContactsWith(node1->right.get(), node2->right.get(), collisionArray);
			}
		}

		/**
		* @brief Function that calculates and stores potential collision pairs that are inside the given node
		* @param node: BVHNode* node type
		* @param collisionArray: std::vector<PotentialContact>& type
		*/
		void getPotentialContacts(BVHNode* node, std::vector<PotentialContact>& collisionArray)
		{
			if (isLeaf(node))
				return;
			
			LIGHT_ASSERT(node->left != nullptr && node->right != nullptr, "Invalid BVH. Please open a bug report if you can reproduce this issue.");
			
			BroadphaseImpl::getPotentialContacts(node->left.get(), collisionArray);
			BroadphaseImpl::getPotentialContacts(node->right.get(), collisionArray);
			BroadphaseImpl::getPotentialContactsWith(node->left.get(), node->right.get(), collisionArray);
		}
	}

	//Hook into the getPotentialContacts function for the outside world.
	//The implementation is kept hidden by using yet another namespace.
	/**
	* @brief Function that calculates and stores potential collision pairs.
	* @param node: BVHNode* node type
	*/
	std::vector<PotentialContact> getPotentialContacts(BVHNode* node)
	{
		std::vector<PotentialContact> v;
		BroadphaseImpl::getPotentialContacts(node, v);
		return v;
	}

	std::vector<PotentialContact> getPotentialContacts(BVH bvh){
		return getPotentialContacts(bvh.getHeadNode().get());
	}
}
