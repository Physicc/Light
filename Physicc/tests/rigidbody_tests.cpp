#include "gtest/gtest.h"
#include "rigidbody.hpp"

class RigidBodyTests : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            testBody = new Physicc::RigidBody(20, glm::vec3(1), 1.0f);
        }

        void TearDown() override
        {
            delete testBody;
        }

        Physicc::RigidBody* testBody;
};

TEST_F(RigidBodyTests, CheckVelocity)
{
    EXPECT_EQ(testBody->getVelocity(), glm::vec3(1.0f)) << "Unable to initialize velocity";
}

TEST_F(RigidBodyTests, ChangeVelocity)
{
    testBody->setVelocity(glm::vec3(4.0f));
    EXPECT_EQ(testBody->getVelocity(), glm::vec3(4.0)) << "Unable to change velocity";
}

TEST_F(RigidBodyTests, AABBTest)
{
    typedef Physicc::BVImpl::BoxBV<Physicc::BVImpl::AABB> BoxBV;
    typedef Physicc::BVImpl::BaseBV<BoxBV,Physicc::BVImpl::AABB> BaseBV;
    
    BaseBV testAABB{glm::vec3(0),glm::vec3(0)};
    float eps = 1e-7;
    BaseBV testAABB2{glm::vec3(eps),glm::vec3(eps)};
    EXPECT_EQ(testAABB2, testAABB) << "Incorrect AABB returned for RigidBody";
    //std::cout<<"------------------"<<glm::all(glm::epsilonEqual(glm::vec3(0),glm::vec3(eps),1e-5f))<<"------------------\n";
}