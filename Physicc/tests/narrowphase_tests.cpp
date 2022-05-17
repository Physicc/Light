#include "gtest/gtest.h"
#include "broadphase.hpp"
#include "bvh.hpp"
#include "narrowphase.hpp"
#include <memory>

class NarrowphaseTests : public ::testing::Test {
	protected:
		void SetUp() override
		{
			double defaultMass = 1.0;
			double defaultVelocity = 1.0;

			// Generic RigidBodies with unique masses and velocities
			Physicc::RigidBody r0(defaultMass, defaultVelocity);
			Physicc::RigidBody r1(defaultMass + 1, defaultVelocity + 3);
			Physicc::RigidBody r2(defaultMass + 2, defaultVelocity + 2);
			Physicc::RigidBody r3(defaultMass + 3, defaultVelocity + 1);
		}

		void TearDown() override
		{
			testVector.clear();
		}

		std::vector<Physicc::RigidBody> testVector;
};

TEST_F(NarrowphaseTests, CheckSphereSphereCollision) {
	testVector.push_back(r0);
	testVector.push_back(r1);

	collider0 = std::make_shared<SphereCollider>(position = glm::vec3(0));
	collider1 = std::make_shared<SphereCollider>(position = glm::vec3(1, 0, 0));

	testVector[0].setCollider(collider0);
	testVector[1].setCollider(collider1);

	BVH bvh(testVector);
	bvh.buildTree();

	auto potentialContacts = getPotentialContacts(bvh); // single element array
	CollisionDetector collisionDetector(potentialContacts);

	bool ifColliding = collisionDetector.SphereSphere(potentialContacts[0]) // change function to templated function later on

	EXPECT_EQ(ifColliding, true)
}

TEST_F(NarrowphaseTests, CheckBoxSphereCollision) {
	testVector.push_back(r0);
	testVector.push_back(r1);

	collider0 = std::make_shared<BoxCollider>(position = glm::vec3(0));
	collider1 = std::make_shared<SphereCollider>(position = glm::vec3(1, 0, 0));

	testVector[0].setCollider(collider0);
	testVector[1].setCollider(collider1);

	BVH bvh(testVector);
	bvh.buildTree();

	auto potentialContacts = getPotentialContacts(bvh); // single element array
	CollisionDetector collisionDetector(potentialContacts);

	bool ifColliding = collisionDetector.BoxSphere(potentialContacts[0]) // change function to templated function later on

	EXPECT_EQ(ifColliding, true)
}

TEST_F(NarrowphaseTests, CheckBoxBoxCollision) {
	testVector.push_back(r0);
	testVector.push_back(r1);

	collider0 = std::make_shared<BoxCollider>(position = glm::vec3(0));
	collider1 = std::make_shared<BoxCollider>(position = glm::vec3(1, 0, 0));

	testVector[0].setCollider(collider0);
	testVector[1].setCollider(collider1);

	BVH bvh(testVector);
	bvh.buildTree();

	auto potentialContacts = getPotentialContacts(bvh); // single element array
	CollisionDetector collisionDetector(potentialContacts);

	bool ifColliding = collisionDetector.BoxBox(potentialContacts[0]) // change function to templated function later on

	EXPECT_EQ(ifColliding, true)
}
