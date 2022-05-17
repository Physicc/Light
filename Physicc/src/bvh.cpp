#include "tools/Tracy.hpp"
/**
 * @file bvh.cpp
 * @brief Constructs a BVH given a list of RigidBody objects.
 * @author Divyansh Tiwari (divyanshtiwari237)
 * @author Tirthankar mazumder (wermos)
 * @bug No known bugs.
 */

/* -- Includes -- */
/*bvh header*/

#include "bvh.hpp"

#include <utility>
#include <algorithm>

namespace Physicc
{

	BVH::BVH(std::vector<RigidBody> rigidBodyList)
		: 	m_head(nullptr),
			m_rigidBodyList(std::move(rigidBodyList)) 
	{
	}

	BoundingVolume::AABB BVH::computeBV(int start, int end)
	{
		ZoneScoped;

		BoundingVolume::AABB bv(m_rigidBodyList[start].getAABB());

		for (auto i = start + 1; i != end; i++)
		{
			bv = BoundingVolume::enclosingBV(bv, m_rigidBodyList[i].getAABB());
			//TODO: Object slicing is might be happening here. Investigate.
		}

		return bv;
	}

	void BVH::sort(Axis axis, int start, int end)
	{
		if (axis == X)
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
			          std::next(m_rigidBodyList.begin(), end),
			          [](const RigidBody& rigidBody1,
			             const RigidBody& rigidBody2) {
			            return rigidBody1.getCentroid().x
				            > rigidBody2.getCentroid().x;
			          });
		} else if (axis == Y)
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
			          std::next(m_rigidBodyList.begin(), end),
			          [](const RigidBody& rigidBody1,
			             const RigidBody& rigidBody2) {
			            return rigidBody1.getCentroid().y
				            > rigidBody2.getCentroid().y;
			          });
		} else
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
			          std::next(m_rigidBodyList.begin(), end),
			          [](const RigidBody& rigidBody1,
			             const RigidBody& rigidBody2) {
			            return rigidBody1.getCentroid().y
				            > rigidBody2.getCentroid().y;
			          });
		}
	}

	BVH::Axis BVH::getMedianCuttingAxis(int start, int end)
	{
		//TODO: Suggest a better name

		glm::vec3 min(m_rigidBodyList[start].getCentroid()),
			max(m_rigidBodyList[start].getCentroid());

		for (int i = start + 1; i <= end; i++)
		{
			min = glm::min(min, m_rigidBodyList[i].getCentroid());
			max = glm::max(max, m_rigidBodyList[i].getCentroid());
		}

		float x_spread = max.x - min.x, y_spread = max.y - min.y,
			z_spread = max.z - min.z;

		if (x_spread >= y_spread && x_spread >= z_spread)
		{
			return Axis::X;
		} else if (y_spread >= x_spread && y_spread >= z_spread)
		{
			return Axis::Y;
		} else
		{
			return Axis::Z;
		}
	}

	inline void BVH::buildTree()
	{
		buildTree(m_head, 0, m_rigidBodyList.size() - 1);
	}


	void BVH::buildTree(BVHNode* node, int start, int end)
	{
		ZoneScoped;

		//implicit convention:
		//no children = leaf node
		//no parent = head node

		//All nodes are equal to nullptr until they are explicitly assigned
		//non-null values

		if (start == end)
		{
			//then the only element left in this sliced vector is the one at
			//`start`
			node->volume = m_rigidBodyList[start].getAABB();
			node->body = &m_rigidBodyList[start];
		} else
		{
			node->volume = BoundingVolume::AABB(computeBV(start, end));

			sort(getMedianCuttingAxis(start, end), start, end);

			auto leftNode = new BVHNode;
			auto rightNode = new BVHNode;

			node->left = leftNode;
			node->right = rightNode;

			leftNode->parent = node;
			rightNode->parent = node;

			buildTree(leftNode, start, start + (end - start) / 2);
			buildTree(rightNode, start + 1 + (end - start) / 2, end);
		}
	}
}
