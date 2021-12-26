#include "gtest/gtest.h"
#include "broadphase.hpp"
#include "bvh.hpp"
#include "glm/gtx/string_cast.hpp"

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
    std::vector<Physicc::RigidBody*> calc = test_bvh.convert();
    std::vector<Physicc::RigidBody*> ans {&test_vector[0],&test_vector[1],&test_vector[2]};
    // for(auto x : calc){
    //     std::cout<<glm::to_string(x->getCentroid())<<std::endl;
    // }
    EXPECT_EQ(ans,calc);
}

