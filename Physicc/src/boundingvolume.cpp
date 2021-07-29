#include "boundingvolume.hpp"

namespace Physicc
{
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