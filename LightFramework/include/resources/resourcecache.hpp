#ifndef __RESOURCECACHE_H__
#define __RESOURCECACHE_H__

#include <memory>
#include <unordered_map>
#include "resources/resource.hpp"

namespace Light
{
	class ResourceCacheBase
	{
	public:
		ResourceCacheBase(ResourceType type, std::string fallback = "") : m_type(type), m_fallback(fallback) {}
		virtual ~ResourceCacheBase() = default;

		ResourceType getType() const { return m_type; }

		template<class T, typename Func, typename... Args>
		std::shared_ptr<T> get(const std::string& handle, Func&& func, Args... args)
		{
			static_assert(std::is_same_v<std::invoke_result_t<decltype(func), Args...>, std::unique_ptr<T> >, "Func must return unique_ptr<T>");

			auto it = m_resources.find(handle);
			if (it != m_resources.end())
			{
				return std::static_pointer_cast<T>(it->second);
			}

			std::shared_ptr<T> ptr = func(args...);
			m_resources.emplace(handle, ptr);
			return ptr;
		}

		template<class T>
		std::shared_ptr<T> get(const std::string& handle)
		{
			auto it = m_resources.find(handle);
			if (it != m_resources.end())
			{
				return std::static_pointer_cast<T>(it->second);
			}

			if (!m_fallback.empty())
			{
				return get<T>(m_fallback);
			}

			return nullptr;
		}

		inline bool exists(const std::string& handle) const { return m_resources.find(handle) != m_resources.end(); }

		inline void unload(const std::string& handle) { m_resources.erase(handle); }

		inline void clear() { m_resources.clear(); }

		inline void setFallback(const std::string& fallback) { m_fallback = fallback; }

	private:
		std::unordered_map<std::string, std::shared_ptr<ResourceBase>> m_resources;
		ResourceType m_type;
		std::string m_fallback;
	};

	template<class T>
	class ResourceCache final : public ResourceCacheBase
	{
		static_assert(std::is_base_of<ResourceBase, T>::value, "T must be derived from ResourceBase");
	public:
		ResourceCache(std::string fallback = "") : ResourceCacheBase(T::getStaticType(), fallback) {}

		virtual ~ResourceCache() = default;

		template<typename Func, typename... Args>
		std::shared_ptr<T> get(const std::string& handle, Func&& func, Args... args)
		{
			static_assert(std::is_same_v<std::invoke_result_t<decltype(func), Args...>, std::unique_ptr<T> >, "Func must return unique_ptr<T>");
			return ResourceCacheBase::get<T>(handle, func, args...);
		}

		std::shared_ptr<T> get(const std::string& handle)
		{
			return ResourceCacheBase::get<T>(handle);
		}

	};
}

#endif // __RESOURCECACHE_H__
