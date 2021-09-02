#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "glm/glm.hpp"
#include <functional>

namespace Physicc
{
	//Named namespace, to keep the implementation hidden from users (or at least
	//make it harder to find)
	namespace BVImplementation
	{
		/**
		 * @brief Axis Aligned Bounding Box
		 *
		 * Helper struct to store diagonally opposite points of the AABB
		 *
		 */
		struct AABB
		{
			glm::vec3 lowerBound;
			glm::vec3 upperBound;
		};

		/**
		 * A templated class that defines the Bounding Volume (BV) of an object, but
		 * in a way that allows others to hot swap actual bounding volumes (like
		 * AABBs, OBBs, 8-DOPs, etc.).
		 *
		 * TODO: Figure out if this is good enough as a description of the
		 * BV class.
		 *
		 * @tparam Derived
		 * TODO: Update this Doxygen comment
		 */
		template <typename Derived, typename BoundingObject>
		class BV
		{
				//CRTP (Curiously Recurring Template Pattern)
			public:
				/**
	 			 * @brief Copy constructor for BoundingVolume
	 			 *
	 			 * @tparam BV The object to be copied
	 			 * @param bv A BV object
	 			 */
				BV(const BV<Derived, BoundingObject>& bv) {
					typeCast()->setVolume(bv.typeCast()->m_volume);
				}

				/**
				 * @brief A constructor for BV which takes a BoundingObject
				 * (like the AABB struct) as a parameter
				 *
				 * @tparam BoundingObject The bounding volume struct to be used
				 * (AABB, OBB, etc.)
				 * @param volume
				 */
				BV(const BoundingObject& volume) {
					typeCast()->setVolume(volume);
				}

				BV(const glm::vec3& lowerBound, const glm::vec3& upperBound) {
					typeCast()->setVolume(lowerBound, upperBound);
				}

				/**
 				 * @brief Returns whether two BVs are overlapping or not
 				 *
 				 * @param bv A BV object
 				 * @return true if the BoundingVolumes are intersecting, and false otherwise
 				 * TODO: Is this, as a return type description, fine?
 				 */
				[[nodiscard]] inline bool overlapsWith(const BV<Derived, BoundingObject>& bv) {
					return typeCast()->overlapsWith(static_cast<Derived&>(bv));
				}
				//implicit contract: all child classes of BV must have this
				//function implemented

				[[nodiscard]] inline float getVolume() {
					return typeCast()->getVolume();
				}
				//Since template instantiations are lazy, a (child) class that
				//doesn't have getVolume() defined simply won't generate any
				//code for this function. However, this also means that trying
				//to use this function will likely result in 5 pages of opaque
				//errors.

			private:
				[[nodiscard]] inline Derived* typeCast() {
					return static_cast<Derived*>(this);
				}
				//A helper function just to make reading the code easier
		};
	}

		template <typename T>
		class BoxBV : public BV<BoxBV<T>, T>
		{
				//CRTP (Curiously Recurring Template Pattern)
				//The overall idea is to create a more specific BV with more box-specific
				//functions, to make the use of BVs easier.
			public:
				BoxBV() = default;

				inline void setVolume(const T& volume) {
					m_volume = volume;
				}

				inline void setVolume(const glm::vec3& lowerBound, const glm::vec3& upperBound) {
					m_volume = T({lowerBound, upperBound});
					//implicit contract: any BoxBV will have a struct that has a
					//lowerBound and upperBound `glm::vec3`s.
				}

				inline float getVolume(BoxBV<T>& bv) {
					//[[nodiscard]] is not needed here because this function is
					//never called by the end user. It is simply called by BV
					//itself, which doesn't actually discard the return type, so
					//we're good.
					return (bv.upperBound.x - bv.lowerBound.x)
						* (bv.upperBound.y - bv.lowerBound.y)
						* (bv.upperBound.z - bv.lowerBound.z);
				}

				inline bool overlapsWith(BoxBV& bv) {
					return (m_volume.lowerBound.x <= bv.m_volume.upperBound.x && m_volume.upperBound.x >= bv.m_volume.lowerBound.x)
						&& (m_volume.lowerBound.y <= bv.m_volume.upperBound.y && m_volume.upperBound.y >= bv.m_volume.lowerBound.y)
						&& (m_volume.lowerBound.z <= bv.m_volume.upperBound.z && m_volume.upperBound.z >= bv.m_volume.lowerBound.z);
				}

			private:
				T m_volume;
		};

	namespace BoundingVolume
	{
		typedef BVImplementation::BV<BoxBV<BVImplementation::AABB>, BVImplementation::AABB> AABB;
	}
}

#endif //__BOUNDINGVOLUME_H__
