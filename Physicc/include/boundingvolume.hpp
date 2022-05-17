#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "tools/Tracy.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"

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
		template <typename Derived, typename BoundingObject>
		class BaseBV
		{
				//CRTP (Curiously Recurring Template Pattern)
			public:
				BaseBV() = default;

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
					ZoneScoped;

					typeCast()->setVolume(volume);
				}

				BaseBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					ZoneScoped;

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
					ZoneScoped;

					return constTypeCast()->overlapsWith(static_cast<const Derived&>(bv));
				}
				//implicit contract: all child classes of BaseBV must have this
				//function implemented

				[[nodiscard]] inline float getVolume() const
				{
					ZoneScoped;

					return constTypeCast()->getVolume();
				}
				//Since template instantiations are lazy, a (child) class that
				//doesn't have getVolume() defined simply won't generate any
				//code for this function. However, this also means that trying
				//to use this function will likely result in 5 pages of opaque
				//errors.

				[[nodiscard]] Derived enclosingBV(const BaseBV& bv) const
				{
					ZoneScoped;

					return constTypeCast()->enclosingBV(static_cast<const Derived&>(bv));
				}

				[[nodiscard]] inline bool operator==(const BaseBV& other) const
				{
					return *constTypeCast() == *other.constTypeCast();
				}

				[[nodiscard]] inline bool operator!=(const BaseBV& other) const
				{
					return !(*this == other);
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

			public:
				BoxBV() = default;

				BoxBV(const glm::vec3& lowerBound, const glm::vec3& upperBound)
				{
					ZoneScoped;

					this->m_volume = {lowerBound, upperBound};
				}

				inline void setVolume(const T& volume)
				{
					ZoneScoped;

					this->m_volume = volume;
				}

				inline void setVolume(const glm::vec3& lowerBound,
				                      const glm::vec3& upperBound)
				{
					ZoneScoped;

					this->m_volume = {lowerBound, upperBound};
					//implicit contract: any BoxBV will have a struct that has
					//lowerBound and upperBound `glm::vec3`s.
				}

				inline float getVolume() const
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

				inline bool overlapsWith(const BoxBV& bv) const
				{
					ZoneScoped;

					return (this->m_volume.lowerBound.x <= bv.m_volume.upperBound.x
							&& this->m_volume.upperBound.x >= bv.m_volume.lowerBound.x)
						&& (this->m_volume.lowerBound.y <= bv.m_volume.upperBound.y
							&& this->m_volume.upperBound.y >= bv.m_volume.lowerBound.y)
						&& (this->m_volume.lowerBound.z <= bv.m_volume.upperBound.z
							&& this->m_volume.upperBound.z >= bv.m_volume.lowerBound.z);
				}

				inline BoxBV enclosingBV(const BoxBV& bv) const
				{
					ZoneScoped;

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
		auto inline enclosingBV(const BVImpl::BaseBV<Derived, BoundingObject>& volume1,
								const BVImpl::BaseBV<Derived, BoundingObject>& volume2)
		{
			ZoneScoped;

			return volume1.enclosingBV(volume2);
		}
		//returns the minimal bounding volume that encompasses both of them
	}
}

#endif //__BOUNDINGVOLUME_H__
