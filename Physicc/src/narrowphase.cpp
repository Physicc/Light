#include "narrowphase.hpp"

namespace Physicc
{
    using Iterator = std::vector<Broadphase::PotentialContact>::iterator;


    namespace Narrowphase
    {

        bool CollisionDetector::generateContacts()
        {
            
        }

        std::vector<Narrowphase::Contact> CollisionDetector::getContacts()
        {
        }

        bool SphereSphere(Broadphase::PotentialContact a)
        {
            // returns true if the spheres in `a` are colliding. Otherwise
			// returns false. Also writes the data to the public collision array.

            auto sphere1 = a.rb1;
            auto sphere2 = a.rb2;
            glm::vec3 centre1 = sphere1.getCentroid();
            glm::vec3 centre2 = sphere2.getCentroid();
            float radius1 = sphere1.getCollider()->getRadius();
            float radius2 = sphere2.getCollider()->getRadius();

            return (glm::distance(centre1, centre2) <= radius1 + radius2);
        }

        bool BoxSphere(Broadphase::PotentialContact a)
        {
            // PotentialContact contains two pointers to rigid bodies. 
            // For BoxSphere, the first is a box, the second a sphere. 
            // Returns true if the box and the sphere in `a` are colliding.
			// Otherwise returns false. Also writes the data to the public collision array.
        }
        
        bool BoxBox(Broadphase::PotentialContact a)
        {
            // returns true if boxes the in `a` are colliding. Otherwise
			// returns false. Also writes the data to the public collision array.
        }

    }
}