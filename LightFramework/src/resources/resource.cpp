#include "core/logging.hpp"

#include "resources/resource.hpp"


namespace Light
{
	ResourceType::ResourceType()
		: m_type(m_numTypes++)
	{
	}

	ResourceType::operator int() const
	{
		return m_type;
	}

	bool ResourceType::operator==(const ResourceType& other) const
	{
		return m_type == other.m_type;
	}

	int ResourceType::m_numTypes = 0;

}
