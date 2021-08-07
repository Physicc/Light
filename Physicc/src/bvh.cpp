#include "bvh.hpp"
#include <utility>

namespace Physicc
{
	using Iterator = std::vector<RigidBody>::iterator;

	BVH::BVH(std::vector<RigidBody> rigidBodyList) : m_rigidBodyList(std::move(
		rigidBodyList)), m_head(nullptr)
	{
	}

	inline void BVH::buildTree()
	{
		buildTree(m_head, m_rigidBodyList.begin(), m_rigidBodyList.end());
	}

	BoundingVolume::AABB computeBV(Iterator begin, Iterator end)
	{
		BoundingVolume::AABB bv(begin->getAABB());

		for (auto it = ++begin; it != end; ++it)
		{
			bv = BoundingVolume::enclosingBV(bv, it->getAABB());
		}

		return bv;
	}

	void BVH::buildTree(BVHNode* node, Iterator begin, Iterator end)
	{
		//implicit convention:
		//no children = leaf node
		//no parent = head node

		//All nodes are equal to nullptr until they are explicitly assigned
		//non-null values

		if (std::next(begin, 1) == end)
		{
			//then the only element left in this sliced vector is the one at
			//`begin`
			node->volume = begin->getAABB();
			node->body = std::addressof(*begin);
		} else
		{
			node->volume = BoundingVolume::AABB(computeBV(begin, end));

			//next, we split
		}
	}
}