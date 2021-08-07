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

			//build a tree of the bounding volumes
			void buildTree();

			//convert the tree into a linear data structure
			std::vector<RigidBody>& convert();

		private:
			BVHNode* m_head;
			std::vector<RigidBody> m_rigidBodyList;
	};
}

#endif //__BVH_H__
