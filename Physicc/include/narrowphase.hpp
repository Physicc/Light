#ifndef __NARROWPHASE_H__
#define __NARROWPHASE_H__

#include "glm/glm.hpp"
#include "core/assert.hpp"
#include "rigidbody.hpp"
#include "broadphase.hpp"
#include <vector>
#include <array>
#include <cstddef> // for std::size_t

/**
 * TODO: Write BoxSphere and BoxBox functions, make it so that the functions return all the
 * information required for contact resolution.
 *
 * Fix formatting issues, and reorganize the functions in the collisionFunctionMatrix struct.
 */

namespace Physicc::Narrowphase
{
	struct Contact
	{

		Contact(std::weak_ptr<RigidBody> rb1, std::weak_ptr<RigidBody> rb2, glm::vec3 cP, glm::vec3 cN, double p) 
		: Body1(rb1), Body2(rb2), contactPoint(cP), contactNormal(cN), penetration(p)
		{
		}

		std::weak_ptr<RigidBody> Body1;
		std::weak_ptr<RigidBody> Body2;
		glm::vec3 contactPoint;
		glm::vec3 contactNormal;
		double penetration;
		// Friction has been removed as it is a property of rigid body and not of Contact
	};

	template <typename... CollisionTypes>
	class collisionFunctionMatrix; // forward declaration of the struct

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
		collisionFunctionMatrix m;
	};

	/**
	template <typename FirstBody, typename SecondBody>
	Contact checkCollision(Broadphase::PotentialContact a) {
		LIGHT_ASSERT(true, "Collision checking for the given types is not supported yet. \
						   Please file a bug report if you can reproduce this issue.")
	}
	*/

	// This file includes specific overloads of the above function.
	#include "narrowphase.ipp"

	// The collisionFunctionMatrix is defined here because it needs to be able to see the full
	// definitions of all the functions before it can do its thing
	template <typename... CollisionTypes>
	class collisionFunctionMatrix {
		public:
			typedef bool (*collisionFuncSignature)(); // TODO: Update function signature.

			collisionFunctionMatrix() {
				// When an object of this struct is instatiated, we will construct the
				// matrix.
				constructMatrix<0, 0, CollisionTypes...>();
			}

			std::array<std::array<collisionFuncSignature, sizeof...(CollisionTypes)>,
					   sizeof...(CollisionTypes)>
				matrix;

			std::array<collisionFuncSignature, sizeof...(CollisionTypes)>& operator[](std::size_t idx) {
					return matrix[idx];
			}

	   private:
		  void constructMatrix() {
			  matrix[row][col] = convert<Head, Head>;
  
			  if constexpr (sizeof...(Rest) > 0) {
							  constructRow<row, col + 1, Head, Rest...>();
							  constructCol<row + 1, col, Head, Rest...>();
				  constructMatrix<row + 1, col + 1, Rest...>();
			  }
		  }
  
		  template <std::size_t row, std::size_t col, typename Head, typename Next,
				    typename... Rest>
		  void constructRow() {
			  matrix[row][col] = convert<Head, Next>;
			  if constexpr (sizeof...(Rest) > 0) {
				  constructRow<row, col + 1, Head, Rest...>();
			  }
		  }
  
		  template <std::size_t row, std::size_t col, typename Head, typename Next,
				    typename... Rest>
		  void constructCol() {
			  matrix[row][col] = convert<Next, Head>;
			  if constexpr (sizeof...(Rest) > 0) {
				  constructCol<row + 1, col, Head, Rest...>();
			  }
		  }
	};
}

#endif //__NARROWPHASE_H__
