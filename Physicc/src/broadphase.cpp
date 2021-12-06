/**
 * @file broadphase.cpp
 * @brief The Broadphase implimentation.
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

namespace Physicc
{
	namespace Broadphase
	{  
		//Anonymous namespace to make the implementation of the function invisible
		//outside this source file
		namespace BroadphaseImpl
		{
			//inlined function to improve readability
			/**	 		
	 		* @brief Inlined function used to check whether the given node is a leaf node
			* @param node: input, BVHNode* node type
	 		*/
			[[nodiscard]] inline bool isLeaf(BVHNode* node)
			{
				return (node->left == nullptr) && (node->right == nullptr);
			}

			/**
	 		* @brief Function that calculates and stores potential collision pairs between two the nodes provided
			* @param node1: input,BVHNode* node type 
			* @param node2: input,BVHNode* node type
			* @param collisionArray: input,std::vector<PotentialContact>& type
	 		*/
			void getPotentialContactsWith(BVHNode* node1, BVHNode* node2, std::vector<PotentialContact>& collisionArray)
			{
				if (isLeaf(node1) && isLeaf(node2))
				{
					//If both nodes are leaves, then we have a PotentialContact, and we add the collision pair to the collisionArray
					collisionArray.push_back(PotentialContact(*(node1->body), *(node2->body)));
				} else if (isLeaf(node1) && !isLeaf(node2))
				{
					//If only of the nodes is a leaf, then make sure that node2 is the leaf node and that we recurse through node1
					getPotentialContactsWith(node2, node1, collisionArray);
					//TODO: #42 @dropTableUsers42 Discuss and figure out if the (minor?) performance penalty is worth it, or if we should just go for the following
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

			/**
	 		* @brief Function that calculates and stores potential collision pairs that are inside the given node
			* @param node: input,BVHNode* node type
			* @param collisionArray: input,std::vector<PotentialContact>& type
	 		*/
			void getPotentialContacts(BVHNode* node, std::vector<PotentialContact>& collisionArray)
			{
				if (isLeaf(node))
					return;
				
				LIGHT_ASSERT(node->left != nullptr && node->right != nullptr, "Invalid Node");
				
				BroadphaseImpl::getPotentialContacts(node->left, collisionArray);
				BroadphaseImpl::getPotentialContacts(node->right, collisionArray);

				if ((node->left)->volume.overlapsWith((node->right)->volume))
				{
					BroadphaseImpl::getPotentialContactsWith(node->left, node->right, collisionArray);
				}
			}
		}

		//Hook into the getPotentialContacts function for the outside world.
		//The implementation is kept hidden (i.e. local to this source file) by
		//using an anonymous namespace.

		/**
	 	* @brief Function that calculates and stores potential collision pairs.
		* @param node: input,BVHNode* node type
	 	*/
		std::vector<PotentialContact> getPotentialContacts(BVHNode* node)
		{
			std::vector<PotentialContact> v;
			getPotentialContacts(node,v);
			return v;
		}
	}
}
