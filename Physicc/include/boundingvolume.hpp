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

	template<class T>
	class BoundingVolume
	{
		public:
			bool overlapsWith(BoundingVolume<T> v);

		private:
			T m_volume;
	};
}

#endif //__BOUNDINGVOLUME_H__
