#include "bvh.hpp"
#include "core/assert.hpp"
#include <utility>
#include <algorithm>
#include <stack>

namespace Physicc
{
	using Iterator = std::vector<RigidBody>::iterator;

	BVH::BVH(std::vector<RigidBody> rigidBodyList) : m_rigidBodyList(std::move(
		rigidBodyList))
	{
		m_head = new BVHNode;
	}

	BoundingVolume::AABB BVH::computeBV(int start, int end)
	{
		BoundingVolume::AABB bv(m_rigidBodyList[start].getAABB());

		for (int i = start + 1; i != end; i++)
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
			            return rigidBody1.getCentroid().z
				            > rigidBody2.getCentroid().z;
			          });
		}
	}

	BVH::Axis BVH::getMedianCuttingAxis(int start, int end)
	{
		//TODO: Suggest a better name
//		float x_min = 0, x_max = 0;
//		float y_min = 0, y_max = 0;
//		float z_min = 0, z_max = 0;

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

//	auto BVH::partitionRigidBodies(Iterator begin,
////	                               Iterator end,
////	                               BoundingVolume::AABB)
////	{
////		glm::vec3 centroidMean(0);
////
////		for (auto it = begin; it != end; ++it) {
////			centroidMean += it->getCentroid();
////		}
////
////		centroidMean /= std::distance(begin, end);
////
////		//check how good the x-axis is, as a splitting axis.
////		float leftVolumeSumX = 0;
////		float rightVolumeSumX = 0;
////
////		int leftObjectsX = 0;
////		int rightObjectsX = 0;
////		int splitObjectsX = 0;
////
////
////	}

	void BVH::buildTree(BVHNode* node, int start, int end)
	{
		//implicit convention:
		//no children = leaf node
		//no parent = head node

		//All nodes are equal to nullptr until they are explicitly assigned
		//non-null values

		LIGHT_ASSERT(start > end, "Pray to god that Jesus helps you, for start is greater than end.");
		if (start == end)
		{
			//then the only element left in this sliced vector is the one at
			//`start`
			node->volume = m_rigidBodyList[start].getAABB();
			node->body = &m_rigidBodyList[start];
		} else
		{
			node->volume = computeBV(start, end);

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
	
	std::vector<RigidBody*> BVH::convert()
	{
		std::stack<BVHNode*> s;
		std::vector<RigidBody*> tree;
		BVHNode* curr = m_head;
		while(!s.empty() || curr != NULL)
		{
			
			while(curr != NULL)
			{
				s.push(curr);
				curr = curr->left;
			}

			curr = s.top();
			s.pop();

			if(curr->left == NULL && curr->right == NULL)
			{
				tree.push_back(curr->body);
			}
			curr = curr->right;
		}
		return tree;
	}

}


////next, we split the vector of objects based on our heuristic
//auto[partitionIndex, axis] = partitionRigidBodies(begin,
//                                                  end,
//                                                  node->volume);
////TODO: How to implement stop criteria? Should we implement them?
//
//if (partitionIndex != begin)
//{
//BVHNode* newNode = new BVHNode;
//buildTree(newNode, begin, partitionIndex);
//}
//if (partitionIndex != end)
//{
//BVHNode* newNode = new BVHNode;
//buildTree(newNode, partitionIndex, end);
//}
