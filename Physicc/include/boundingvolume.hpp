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
}

#endif //__BOUNDINGVOLUME_H__
