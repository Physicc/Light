#include "bvh.hpp"

namespace Physicc
{
	BVH::BVH(std::vector<RigidBody> rigidBodyList) : m_rigidBodyList(
		rigidBodyList), m_head(nullptr)
	{
	}

	void BVH::buildTree()
	{
		buildTree();
	}

	BoundingVolume<AABB> computeBoundingVolume(std::vector<RigidBody>::iterator begin,
											   std::vector<RigidBody>::iterator end) {

		glm::vec3 lowerBound, upperBound;

		for (std::vector<RigidBody>::iterator it = begin; it != end; ++it) {
			BoundingVolume<AABB> bv = it->getAABB();

			lowerBound.x = std::min(lowerBound.x, bv.m_volume.lowerBound.x);
			lowerBound.y = std::min(lowerBound.y, bv.m_volume.lowerBound.y);
			lowerBound.z = std::min(lowerBound.z, bv.m_volume.lowerBound.z);

			lowerBound.x = std::min(lowerBound.x, bv.m_volume.lowerBound.x);
			lowerBound.y = std::min(lowerBound.y, bv.m_volume.lowerBound.y);
			lowerBound.z = std::min(lowerBound.z, bv.m_volume.lowerBound.z);
		}
	}
	void BVH::buildTree(BVHNode* node,
						std::vector<RigidBody>::iterator begin,
						std::vector<RigidBody>::iterator end)
	{
		//implicit convention:
		//no children = leaf node
		//no parent = head node
	}
}
