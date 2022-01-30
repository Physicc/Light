#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "tools/Tracy.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

#ifdef __cpp_lib_concepts
#include <concepts>
#endif

namespace Physicc
{
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
			//POD to a non-POD type.
			glm::vec3 lowerBound;
			glm::vec3 upperBound;
		};

#ifdef __cpp_lib_concepts
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
		 * in a way that allows hot swapping actual bounding volumes (like AABBs, OBBs, 8-DOPs,
		 * etc.).
		 *
		 * TODO: Figure out if this is good enough as a description of the
		 * BV class.
		 *
		 * @tparam Derived
		 * @tparam BoundingObject
		 * TODO: Update this Doxygen comment
		 */
#ifdef __cpp_lib_concepts
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
					typeCast()->setBoundingVolumeImpl(volume);
				}

				BaseBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					typeCast()->setBoundingVolumeImpl(lowerBound, upperBound);
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
					return constTypeCast()->overlapsWithImpl(static_cast<const Derived&>(bv));
				}
				//implicit contract: all child classes of BaseBV must have this
				//function implemented

				[[nodiscard]] inline float getVolume() const
				{
					return constTypeCast()->getVolumeImpl();
				}
				//Since template instantiations are lazy, a (child) class that
				//doesn't have getVolumeImpl() defined simply won't generate any
				//code for this function. However, this also means that trying
				//to use this function will likely result in 5 pages of opaque
				//errors.

				[[nodiscard]] inline glm::vec3 getLowerBound() const
				{
					return constTypeCast()->getUpperBoundImpl();
				}

				[[nodiscard]] inline glm::vec3 getUpperBound() const
				{
					return constTypeCast()->getUpperBoundImpl();
				}
				//For the same reason as before, the above two functions can safely
				//reside in the base class.

				[[nodiscard]] Derived enclosingBV(const BaseBV& bv) const
				{
					return constTypeCast()->enclosingBVImpl(static_cast<const Derived&>(bv));
				}

			private:
				[[nodiscard]] inline Derived* typeCast()
				{
					return static_cast<Derived*>(this);
				}
				//A helper function just to make reading the code easier

				[[nodiscard]] inline const Derived* constTypeCast() const
				{
					return static_cast<const Derived*>(this);
				}
				//Another helper function just to make reading the code easier
			protected:
				BoundingObject m_volume;
				//For accessing non-templated member variables (or even functions) of a templated class
				//inside a derived class, make sure to use "this" pointer.
				//See here why: https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members
		};

		template <typename T>
		class BoxBV : public BaseBV<BoxBV<T>, T>
		{
				//CRTP (Curiously Recurring Template Pattern)
				//The overall idea is to create a more specific BV with more box-specific
				//functions, to make the use of BVs easier.
			private:
				//Hiding the exact implementation by making all the functions
				//private

				friend BaseBV<BoxBV<T>, T>;

				BoxBV() = default;

				BoxBV(const BoxBV& bv) = default;

				BoxBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					this->m_volume = {lowerBound, upperBound};
				}

				inline void setBoundingVolumeImpl(const T& volume)
				{
					this->m_volume = volume;
				}

				inline void setBoundingVolumeImpl(const glm::vec3& lowerBound,
												  const glm::vec3& upperBound)
				{
					this->m_volume = {lowerBound, upperBound};
					//implicit contract: any BoxBV will have a struct that has
					//lowerBound and upperBound `glm::vec3`s.
				}

				inline float getVolumeImpl() const
				{
					ZoneScoped;

					//[[nodiscard]] is not needed here because this function is
					//never called by the end user. It is simply called by BV
					//itself, which doesn't actually discard the return type, so
					//we're good.
					return (this->m_volume.upperBound.x - this->m_volume.lowerBound.x)
						* (this->m_volume.upperBound.y - this->m_volume.lowerBound.y)
						* (this->m_volume.upperBound.z - this->m_volume.lowerBound.z);
				}

				inline glm::vec3 getLowerBoundImpl() const
				{
					return this->m_volume.lowerBound;
				}

				inline glm::vec3 getUpperBoundImpl() const
				{
					return this->m_volume.upperBound;
				}

				inline bool overlapsWithImpl(const BoxBV& bv) const
				{
					return (this->m_volume.lowerBound.x <= bv.m_volume.upperBound.x
							&& this->m_volume.upperBound.x >= bv.m_volume.lowerBound.x)
						&& (this->m_volume.lowerBound.y <= bv.m_volume.upperBound.y
							&& this->m_volume.upperBound.y >= bv.m_volume.lowerBound.y)
						&& (this->m_volume.lowerBound.z <= bv.m_volume.upperBound.z
							&& this->m_volume.upperBound.z >= bv.m_volume.lowerBound.z);
				}

				inline BoxBV enclosingBVImpl(const BoxBV& bv) const
				{
					return {glm::min(this->m_volume.lowerBound, bv.m_volume.lowerBound),
						glm::max(this->m_volume.upperBound, bv.m_volume.upperBound)};
				}
				//As per our previous implicit contract, these `glm::vec3`s are
				//guaranteed to exist, so this is legal.
		};
	}

	namespace BoundingVolume
	{
		typedef BVImpl::BoxBV<BVImpl::AABB> AABB;

		template <typename Derived, typename BoundingObject>
		inline auto enclosingBV(const BVImpl::BaseBV<Derived, BoundingObject>& volume1,
								const BVImpl::BaseBV<Derived, BoundingObject>& volume2)
		{
			ZoneScoped;

			return volume1.enclosingBV(volume2);
		}
		// returns the minimal bounding volume that encompasses both of them
	}
}

#endif //__BOUNDINGVOLUME_H__
