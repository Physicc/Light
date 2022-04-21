#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "light/rendering/buffer.hpp"

#include "core/logging.hpp"

namespace Light
{
	struct ResourceType
	{
	public:
		ResourceType();
		ResourceType(const ResourceType& other) = default;
		ResourceType(ResourceType&& other) = default;

		~ResourceType() = default;

		ResourceType& operator=(const ResourceType& other) = default;
		ResourceType& operator=(ResourceType&& other) = default;

		bool operator==(const ResourceType& other) const;

		operator int() const;
	private:
		static int m_numTypes;
		const int m_type;
	};

	class ResourceBase
	{
	public:
		virtual ~ResourceBase() = default;
		virtual ResourceType getType() const = 0;
	};

	template<class T>
	class Resource : public ResourceBase
	{
	private:
		Resource() = default;
		friend T;

	public:
		static ResourceType getStaticType() { return m_type; }
		ResourceType getType() const override { return m_type; }

	private:
		static ResourceType m_type;
	};

	template<class T>
	ResourceType Resource<T>::m_type;

};

#define RESOURCE_CLASS(name) \
	class name final : public Light::Resource<name>


#endif // __RESOURCE_H__
