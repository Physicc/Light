#include "gtest/gtest.h"
#include "broadphase.hpp"
#include "glm/gtx/string_cast.hpp"
#include <memory>
#include <vector>
#include<iostream>

class BroadphaseTests : public ::testing::Test {
	protected:
		void SetUp() override
		{
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));

			testVector[0].setCollider(std::make_shared<Physicc::SphereCollider>(2, glm::vec3(1, 0, 0)));
			testVector[1].setCollider(std::make_shared<Physicc::SphereCollider>(2, glm::vec3(0, 0, 0)));
			testVector[2].setCollider(std::make_shared<Physicc::SphereCollider>(2, glm::vec3(0, 1, 0)));

		}

		void TearDown() override
		{
			testVector.clear();
		}

		std::vector<Physicc::RigidBody> testVector;
};

TEST_F(BroadphaseTests, CheckPotentialContacts) {
    Physicc::BVH testBVH(testVector);
	testBVH.buildTree();
    std::vector<PotentialContact> collisionArray = getPotentialContacts(testBVH);
    std::vector<PotentialContact> answer;
    std::cout<<collisionArray.size();
    answer.emplace_back(testVector[0], testVector[1]);
    answer.emplace_back(testVector[1], testVector[2]);
    answer.emplace_back(testVector[2], testVector[1]);
}



