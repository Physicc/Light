/**
 * @file boundingvolume.cpp
 * @brief Contains the implementation of the BoundingVolume class
 *
 * The BoundingVolume file contains the BoundingVolume class and the AABB
 * struct, which are used to define the bounding volumes which contain
 * RigidBody's.
 *
 * @author Tirthankar Mazumder (wermos)
 * @bug No known bugs.
 */

/* -- Includes -- */
/* boundingvolume header */
#include "boundingvolume.hpp"

namespace Physicc
{
	/**
	 * @brief Copy constructor for BoundingVolume
	 *
	 * @tparam T The bounding volume to be used (AABB, OBB, etc.)
	 * @param v A BoundingVolume object
	 */
	template <class T>
	BoundingVolume<T>::BoundingVolume(const BoundingVolume<T>& v)
	{
		m_volume = v.m_volume;
	}

	/**
	 * @brief A constructor for BoundingVolume which takes a bounding volume
	 * (like AABB) as a parameter
	 *
	 * @tparam T The bounding volume to be used (AABB, OBB, etc.)
	 * @param volume
	 */
	template <class T>
	BoundingVolume<T>::BoundingVolume(const T& volume)
	{
		m_volume = volume;
	}

	/**
	 * @brief Returns whether two AABB BoundingVolumes are overlapping
	 *
	 * A template specialization of the overlapsWith, which returns true or
	 * false, depending on whether or not the two AABB BoundingVolumes are
	 * intersecting
	 *
	 * @param v A BoundingVolume object
	 * @return true if the BoundingVolumes are intersecting, and false otherwise
	 * TODO: Is this, as a return type description, fine?
	 */
	template <>
	bool BoundingVolume<AABB>::overlapsWith(BoundingVolume<AABB> v)
	{
		return (m_volume.lowerBound.x <= v.m_volume.upperBound.x && m_volume.upperBound.x >= v.m_volume.lowerBound.x) &&
			   (m_volume.lowerBound.y <= v.m_volume.upperBound.y && m_volume.upperBound.y >= v.m_volume.lowerBound.y) &&
			   (m_volume.lowerBound.z <= v.m_volume.upperBound.z && m_volume.upperBound.z >= v.m_volume.lowerBound.z);
	}

	template class BoundingVolume<AABB>;
	//Explicitly instantiating the AABB-based BoundingVolume class
}