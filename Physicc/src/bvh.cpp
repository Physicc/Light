#include "bvh.hpp"

namespace Physicc
{
	BVH::BVH(std::vector<RigidBody> rigidBodyList) : m_rigidBodyList(
		rigidBodyList), m_head(nullptr)
	{
	}

	void BVH::buildTree()
	{

	}
}
