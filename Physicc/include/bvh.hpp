#ifndef __BVH_H__
#define __BVH_H__

#include "boundingvolume.hpp"
#include "rigidbody.hpp"
#include <memory>

namespace Physicc
{
	struct BVHNode
	{
		BoundingVolume::AABB volume;
		std::unique_ptr<RigidBody> body;
		//TODO: Ensure that a unique_ptr is the proper smart pointer type for the RigidBody.

		std::shared_ptr<BVHNode> parent;
		std::shared_ptr<BVHNode> left;
		std::shared_ptr<BVHNode> right;
	};

	class BVH
	{
		public:
			BVH(std::vector<RigidBody> rigidBodyList);

			inline void buildTree()
			{
				buildTree(m_head, 0, m_rigidBodyList.size() - 1);
			}
			//build a binary tree of the bounding volumes

			//convert the tree into a linear data structure
			std::vector<RigidBody*> convert();

		private:
			std::shared_ptr<BVHNode> m_head;
			std::vector<RigidBody> m_rigidBodyList;

			BoundingVolume::AABB computeBV(std::size_t start, std::size_t end);

			void buildTree(std::shared_ptr<BVHNode> node, std::size_t start, std::size_t end);

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
