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

/* -- Includes -- */
/*bvh header*/

#include "bvh.hpp"
#include "core/assert.hpp"
#include <utility>
#include <algorithm>
#include <stack>
#include <memory>

namespace Physicc
{
	BVH::BVH(std::vector<RigidBody> rigidBodyList) :
		m_rigidBodyList(rigidBodyList)
	{
		m_head = std::make_shared<BVHNode>();
	}

	BoundingVolume::AABB BVH::computeBV(std::size_t start, std::size_t end)
	{
		BoundingVolume::AABB bv(m_rigidBodyList[start].getAABB());

		for (std::size_t i = start + 1; i != end; i++)
		{
			bv = BoundingVolume::enclosingBV(bv, m_rigidBodyList[i].getAABB());
			//TODO: Object slicing is might be happening here. Investigate.
		}
    
		return bv;
	}

	void BVH::sort(Axis axis, std::size_t start, std::size_t end)
	{
		if (axis == X)
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
						std::next(m_rigidBodyList.begin(), end + 1),
						[](const RigidBody& rigidBody1,
						const RigidBody& rigidBody2) {
						return rigidBody1.getCentroid().x
							> rigidBody2.getCentroid().x;
					});
		} else if (axis == Y)
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
						std::next(m_rigidBodyList.begin(), end + 1),
						[](const RigidBody& rigidBody1,
						const RigidBody& rigidBody2) {
						return 	rigidBody1.getCentroid().y
								> rigidBody2.getCentroid().y;
					});
		} else
		{
			std::sort(std::next(m_rigidBodyList.begin(), start),
						std::next(m_rigidBodyList.begin(), end + 1),
						[](const RigidBody& rigidBody1,
						const RigidBody& rigidBody2) {
						return 	rigidBody1.getCentroid().z
								> rigidBody2.getCentroid().z;
					});
		}
	}

	BVH::Axis BVH::getMedianCuttingAxis(std::size_t start, std::size_t end)
	{
		//TODO: Suggest a better name
    
		glm::vec3 min(m_rigidBodyList[start].getCentroid()),
			max(m_rigidBodyList[start].getCentroid());

		for (std::size_t i = start + 1; i <= end; ++i)
		{
			// glm::min and glm::max find the coordinate-wise mins and maxes.
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

	void BVH::buildTree(std::shared_ptr<BVHNode> node, std::size_t start, std::size_t end)
	{
		ZoneScoped;

		//implicit convention:
		//no children = leaf node
		//no parent = head node

		//All nodes are equal to nullptr until they are explicitly assigned
		//non-null values

		LIGHT_ASSERT(start <= end, "Pray to god that Jesus helps you, for start is greater than end.");

		if (start == end)
		{
			//then the only element left in this sliced vector is the one at
			//`start`
			node->volume = m_rigidBodyList[start].getAABB();
			node->body = std::make_shared<RigidBody>(m_rigidBodyList[start]);
		} else
		{
			node->volume = computeBV(start, end);

			sort(getMedianCuttingAxis(start, end), start, end);

			auto leftNode = std::make_shared<BVHNode>();
			auto rightNode = std::make_shared<BVHNode>();

			node->left = leftNode;
			node->right = rightNode;

			leftNode->parent = node;
			rightNode->parent = node;

			buildTree(leftNode, start, start + (end - start) / 2);
			buildTree(rightNode, start + 1 + (end - start) / 2, end);
		}
	}

	std::vector<std::weak_ptr<RigidBody>> BVH::convert()
	{
		std::stack<BVHNode*> s;
		std::vector<std::weak_ptr<RigidBody>> tree;
		BVHNode* currentNode = m_head.get();
		while (!s.empty() || currentNode != nullptr)
		{

			while (currentNode != nullptr)
			{
				s.push(currentNode);
				currentNode = currentNode->left.get();
			}

			currentNode = s.top();
			s.pop();

			if (currentNode->left == nullptr && currentNode->right == nullptr)
			{
				tree.push_back(currentNode->body);
			}
			currentNode = currentNode->right.get();
		}
		return tree;
	}
}
