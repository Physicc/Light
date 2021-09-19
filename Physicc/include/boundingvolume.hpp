#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "glm/glm.hpp"
#include <concepts>

namespace Physicc
{
	//Verdict: Discuss commented functions (keep or discard? KEEP)

	//Warning: Return types may need the template specified.

	//Named namespace, to keep the implementation hidden from users (or at least
	//make it harder to find)
	namespace BVImpl
	{
		/**
		 * @brief Axis Aligned Bounding Box
		 *
		 * Helper struct to store diagonally opposite points of the AABB
		 *
		 */
		struct AABB
		{
			AABB() = default;
			//lb = lowerBound, ub = upperBound
			AABB(const glm::vec3& lb, const glm::vec3& ub) : lowerBound(lb), upperBound(ub)
			{
			}
			//the existence of this constructor does not promote this struct from a
			//POD to a non-POD type. (at least, it shouldn't.)

			glm::vec3 lowerBound;
			glm::vec3 upperBound;
		};

#if __cplusplus > 201703L
		template <typename Derived, typename BoundingObject>
		concept SpecializedBV = std::derives_from<BaseBV<Derived>>
			&& requires (BaseBV<Derived, BoundingObject> bv, BoundingObject volume)
			{
				std::copy_constructible<BaseBV<Derived, BoundingObject>>;
				std::is_constructible_from(volume);
			}
			&& requires (BaseBV<Derived, BoundingObject> bv1, BaseBV<Derived, BoundingObject bv2)
			{
				bv1.overlapsWith(bv2);
				bv1.enclosingBV(bv2);
			};
#endif

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
#if __cplusplus > 201703L
		template <typename Derived, typename BoundingObject> requires SpecializedBV<Derived, BoundingObject>
#else
		template <typename Derived, typename BoundingObject>
#endif
		class BaseBV
		{
				//CRTP (Curiously Recurring Template Pattern)
			public:
				BaseBV() = default;

				/**
	 			 * @brief Copy constructor for BoundingVolume
	 			 *
	 			 * @tparam BV The object to be copied
	 			 * @param bv A BV object
	 			 */
				BaseBV(const BaseBV& bv) = default;
				//the constructors for the base and child classes are kept
				//simple on purpose, to allow for easy type conversions.

				/**
				 * @brief A constructor for BV which takes a BoundingObject
				 * (like the AABB struct) as a parameter
				 *
				 * @tparam BoundingObject The bounding volume struct to be used
				 * (AABB, OBB, etc.)
				 * @param volume
				 */
				BaseBV(const BoundingObject& volume)
				{
					typeCast()->setVolume(volume);
				}

				BaseBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					typeCast()->setVolume(lowerBound, upperBound);
				}

				/**
 				 * @brief Returns whether two BVs are overlapping or not
 				 *
 				 * @param bv A BaseBV object
 				 * @return true if the BoundingVolumes are intersecting, and false otherwise
 				 * TODO: Is this, as a return type description, fine?
 				 */
				[[nodiscard]] inline bool overlapsWith(const BaseBV& bv) const
				{
					return constTypeCast()->overlapsWith(static_cast<const Derived&>(bv));
				}
				//implicit contract: all child classes of BaseBV must have this
				//function implemented

				[[nodiscard]] inline float getVolume() const
				{
					return constTypeCast()->getVolume();
				}
				//Since template instantiations are lazy, a (child) class that
				//doesn't have getVolume() defined simply won't generate any
				//code for this function. However, this also means that trying
				//to use this function will likely result in 5 pages of opaque
				//errors.

//				[[nodiscard]] inline BoundingObject getBoundingVolume() const
//				{
//					return constTypeCast()->getBoundingVolume();
//				}
				[[nodiscard]] Derived enclosingBV(const BaseBV& bv) const
				{
					return constTypeCast()->enclosingBV(static_cast<const Derived&>(bv));
				}

			private:
				[[nodiscard]] inline Derived* typeCast()
				{
					return static_cast<Derived*>(this);
				}
				//A helper function just to make reading the code easier

//				[[nodiscard]] inline const Derived* constTypeCast() const
//				{
//					return static_cast<const Derived*>(this);
//				}
//				//A helper function just to make reading the code easier

				[[nodiscard]] inline const Derived* constTypeCast() const//constTypeCastToConst() const
				{
					return static_cast<const Derived*>(this);
				}
				//Another helper function just to make reading the code easier
		};

		template <typename T>
		class BoxBV : public BaseBV<BoxBV<T>, T>
		{
				//CRTP (Curiously Recurring Template Pattern)
				//The overall idea is to create a more specific BV with more box-specific
				//functions, to make the use of BVs easier.
			public:
				BoxBV() = default;

				BoxBV(const BoxBV& bv) = default;

				BoxBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					m_volume = {lowerBound, upperBound};
				}

				inline void setVolume(const T& volume)
				{
					m_volume = volume;
				}

				inline void setVolume(const glm::vec3& lowerBound,
				                      const glm::vec3& upperBound)
				{
					m_volume = {lowerBound, upperBound};
//					m_volume.lowerBound = lowerBound;
//					m_volume.upperBound = upperBound;
					//implicit contract: any BoxBV will have a struct that has
					//lowerBound and upperBound `glm::vec3`s.
				}

				inline float getVolume() const
				{
					//[[nodiscard]] is not needed here because this function is
					//never called by the end user. It is simply called by BV
					//itself, which doesn't actually discard the return type, so
					//we're good.
					return (m_volume.upperBound.x - m_volume.lowerBound.x)
						* (m_volume.upperBound.y - m_volume.lowerBound.y)
						* (m_volume.upperBound.z - m_volume.lowerBound.z);
				}

				inline bool overlapsWith(const BoxBV& bv) const
				{
					return (m_volume.lowerBound.x <= bv.m_volume.upperBound.x
							&& m_volume.upperBound.x >= bv.m_volume.lowerBound.x)
						&& (m_volume.lowerBound.y <= bv.m_volume.upperBound.y
							&& m_volume.upperBound.y >= bv.m_volume.lowerBound.y)
						&& (m_volume.lowerBound.z <= bv.m_volume.upperBound.z
							&& m_volume.upperBound.z >= bv.m_volume.lowerBound.z);
				}

//				inline T getBoundingVolume() const
//				{
//					return m_volume;
//				}

				inline BoxBV enclosingBV(const BoxBV& bv) const
				{
					return {glm::min(m_volume.lowerBound, bv.m_volume.lowerBound),
						glm::max(m_volume.upperBound, bv.m_volume.upperBound)};
				}
				//As per our previous implicit contract, these `glm::vec3`s are
				//guaranteed to exist, so this is legal.

			private:
				T m_volume;
		};
	}

	namespace BoundingVolume
	{
		typedef BVImpl::BaseBV<BVImpl::BoxBV<BVImpl::AABB>, BVImpl::AABB> AABB;

		template <typename Derived, typename BoundingObject>
		auto inline enclosingBV(const BVImpl::BaseBV<Derived, BoundingObject>& volume1,
								const BVImpl::BaseBV<Derived, BoundingObject>& volume2)
		{
			return volume1.enclosingBV(volume2);
		}
		//returns the minimal bounding volume that encompasses both of them
	}
}

#endif //__BOUNDINGVOLUME_H__

