#include "gtest/gtest.h"
#include "broadphase.hpp"
#include "bvh.hpp"
#include "glm/gtx/string_cast.hpp"
#include <memory>

class BVH_tests : public ::testing::Test {
	protected:
		void SetUp() override
		{
			test_vector.emplace_back(Physicc::RigidBody{20, glm::vec3(1), 1.0f});
			test_vector.emplace_back(Physicc::RigidBody{20, glm::vec3(1), 1.0f});
			test_vector.emplace_back(Physicc::RigidBody{20, glm::vec3(1), 1.0f});

			test_vector[1].setPosition(glm::vec3(1.0f));
			test_vector[2].setPosition(glm::vec3(2.0f));
		}

		// void TearDown() override{}

		std::vector<Physicc::RigidBody> test_vector;
};

TEST_F(BVH_tests, Check_bvh_tree){
	Physicc::BVH test_bvh(test_vector);
	test_bvh.buildTree();
	std::vector<std::weak_ptr<Physicc::RigidBody>> calc = test_bvh.convert();
	std::vector<glm::vec3> centroids;
	for(auto body:calc){
		auto temp = body.lock();
		centroids.push_back(temp->getCentroid());
	}
	std::vector<glm::vec3> ans {test_vector[2].getCentroid(),test_vector[1].getCentroid(),test_vector[0].getCentroid()};
	// for(auto x : centroids){
	//     std::cout<<glm::to_string(x)<<std::endl;
	// }
	EXPECT_EQ(ans,centroids);
}

