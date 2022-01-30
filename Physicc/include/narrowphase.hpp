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
            }

            std::vector<Narrowphase::Contact> getContacts()
            {
            }

            private:
            std::vector<Broadphase::PotentialContact> collisionArray;
            std::vector<Narrowphase::Contact> collisions;

        };


        Contact SphereSphere(Broadphase::PotentialContact a)
        {
        }

        Contact BoxSphere(Broadphase::PotentialContact a)
        {
        }

        Contact BoxBox(Broadphase::PotentialContact a)
        {
        }
    }
}


#endif //__NARROWPHASE_H__