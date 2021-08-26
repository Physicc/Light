#ifndef __BOUNDINGVOLUME_H__
#define __BOUNDINGVOLUME_H__

#include "glm/glm.hpp"

namespace Physicc
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
	 * A templated class that defines the Bounding Volume of an object, but in
	 * a way that allows others to hot swap actual bounding volumes (like AABBs,
	 * OBBs, 8-DOPs, etc.).
	 * TODO: Figure out if this is good enough as a description of the
	 * BoundingVolume class.
	 *
	 * @tparam T
	 */
	template <class T>
	class BoundingVolume
	{
		public:
			BoundingVolume(const BoundingVolume<T>& v);
			BoundingVolume(const T& volume);

			bool overlapsWith(BoundingVolume<T> v);

		private:
			T m_volume;
	};

	template <>
	bool BoundingVolume<AABB>::overlapsWith(BoundingVolume<AABB> v);
	//Class method specialization of the overlapsWith function

	//TODO: Discuss this design decision
	namespace AABBUtilities
	{
		/**
		 * @brief returns the volume of the AABB
		 *
		 * @return float
		 */
		[[nodiscard]] inline float getVolume(BoundingVolume<AABB>& bv)
		{
			return (bv.upperBound.x - bv.lowerBound.x) *
				   (bv.upperBound.y - bv.lowerBound.y) *
				   (bv.upperBound.z - bv.lowerBound.z);
		}
	}
}

#endif //__BOUNDINGVOLUME_H__
