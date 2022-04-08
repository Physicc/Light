#ifndef __NARROWPHASE_H__
#define __NARROWPHASE_H__

#include "glm/glm.hpp"
#include "rigidbody.hpp"
#include "broadphase.hpp"
#include <vector>

/**
 * TODO: 
 * 
 */

namespace Physicc
{
    namespace Narrowphase
    {
            
        struct Contact
        {

            Contact(glm::vec3 cP, glm::vec3 cN, double p, bool f = false) 
            : contactPoint(cP), contactNormal(cN), penetration(p)
            {
            }

            glm::vec3 contactPoint;
            glm::vec3 contactNormal;
            double penetration;
            // Friction has been removed as it is a property of rigid body and not of Contact
        };

        class CollisionDetector
        {
            public:
            CollisionDetector(std::vector<Broadphase::PotentialContact> &v)
            : collisionArray(v)
            {
            }
            

            // Move individual components to individual function

            bool generateContacts()
            {
				// Take in the entire array of potential contacts and then
				// traverse through every single one, and dispatch it to the
				// correct narrow phase collision detecting function.
            }

            std::vector<Narrowphase::Contact> getContacts()
            {
            }

            private:
            std::vector<Broadphase::PotentialContact> collisionArray;
            std::vector<Narrowphase::Contact> collisionInfo;
			// the above should return a complete list of contact info for every collision
			std::vector<std::pair<RigidBody, RigidBody>>> collision;
        };


        bool SphereSphere(Broadphase::PotentialContact a);

        bool BoxSphere(Broadphase::PotentialContact a);

        bool BoxBox(Broadphase::PotentialContact a);
    }
}


#endif //__NARROWPHASE_H__
