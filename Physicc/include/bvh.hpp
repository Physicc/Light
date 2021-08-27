#ifndef __BVH_H__
#define __BVH_H__

#include "boundingvolume.hpp"
#include "rigidbody.hpp"

namespace Physicc
{
	struct BVHNode
	{
		BoundingVolume<AABB> volume;
		RigidBody* body = nullptr;
		BVHNode* parent = nullptr;
		BVHNode* left = nullptr;
		BVHNode* right = nullptr;
	};

	class BVH
	{
		public:
			BVH(std::vector<RigidBody> rigidBodyList);

			void buildTree();
			//build a tree of the bounding volumes

			std::vector<RigidBody>& convert();
			//convert the tree into a linear data structure

		private:
			BVHNode* m_head;
			std::vector<RigidBody> m_rigidBodyList;

			BoundingVolume<AABB> computeBoundingVolume(std::vector<RigidBody>::iterator begin,
													   std::vector<RigidBody>::iterator end);

			void buildTree(BVHNode* node,
			               std::vector<RigidBody>::iterator begin,
			               std::vector<RigidBody>::iterator end);
	};
}

#endif //__BVH_H__
