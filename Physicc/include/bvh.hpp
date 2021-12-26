#ifndef __BVH_H__
#define __BVH_H__

#include "boundingvolume.hpp"
#include "rigidbody.hpp"

namespace Physicc
{
	struct BVHNode
	{
		BoundingVolume::AABB volume;
		RigidBody* body = nullptr;

		BVHNode* parent = nullptr;
		BVHNode* left = nullptr;
		BVHNode* right = nullptr;
	};

	class BVH
	{
		public:
			BVH(std::vector<RigidBody> rigidBodyList);

			inline void buildTree()
			{
				buildTree(m_head, 0, m_rigidBodyList.size() - 1);
			}
			//build a tree of the bounding volumes

			//convert the tree into a linear data structure
			std::vector<RigidBody*> convert();

		private:
			BVHNode* m_head;
			std::vector<RigidBody> m_rigidBodyList;

			BoundingVolume::AABB computeBV(int start, int end);

			void buildTree(BVHNode* node, int start, int end);

			enum Axis {
				X,
				Y,
				Z,
			};

			void sort(Axis axis, int start, int end);
			Axis getMedianCuttingAxis(int start, int end);
	};
}

#endif //__BVH_H__
