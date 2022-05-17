#include "gtest/gtest.h"
#include "broadphase.hpp"
#include "bvh.hpp"
#include "glm/gtx/string_cast.hpp"
#include <memory>
// #include <iostream>

class BVHTests : public ::testing::Test {
	protected:
		void SetUp() override
		{
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));
			testVector.emplace_back(std::make_shared<Physicc::RigidBody>(20, glm::vec3(1), 1.0f));

			testVector[0].setCollider(std::make_shared<Physicc::BoxCollider>());
			testVector[1].setCollider(std::make_shared<Physicc::BoxCollider>());
			testVector[2].setCollider(std::make_shared<Physicc::BoxCollider>());

			testVector[1].setPosition(glm::vec3(1.0f));
			testVector[2].setPosition(glm::vec3(2.0f));
		}

		void TearDown() override
		{
			testVector.clear();
		}

		std::vector<Physicc::RigidBody> testVector;
};

TEST_F(BVHTests, CheckBVHTree) {
	Physicc::BVH testBVH(testVector);
	testBVH.buildTree();
	std::vector<std::weak_ptr<Physicc::RigidBody>> calc = testBVH.convert();
	std::vector<glm::vec3> centroids;
	for (auto body : calc)
	{
		auto temp = body.lock();
		centroids.push_back(temp->getCentroid());
		// std::cout << "passed\n";
	}
	std::vector<glm::vec3> ans {testVector[2].getCentroid(), testVector[1].getCentroid(), testVector[0].getCentroid()};
	// for (auto x : centroids)
	// {
	//     std::cout << glm::to_string(x) << std::endl;
	// }
	EXPECT_EQ(ans, centroids);
}

