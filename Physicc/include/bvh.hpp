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

			inline void buildTree();
			//build a tree of the bounding volumes

			//convert the tree into a linear data structure
			std::vector<RigidBody>& convert();

		private:
			BVHNode* m_head;
			std::vector<RigidBody> m_rigidBodyList;

			BoundingVolume::AABB computeBV(std::size_t start, std::size_t end);

			void buildTree(BVHNode* node, std::size_t start, std::size_t end);

			enum Axis {
				X,
				Y,
				Z,
			};

			void sort(Axis axis, std::size_t start, std::size_t end);
			Axis getMedianCuttingAxis(std::size_t start, std::size_t end);
	};
}

#endif //__BVH_H__
