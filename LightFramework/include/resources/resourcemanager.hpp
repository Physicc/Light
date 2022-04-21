#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <vector>

#include "resources/resourcecache.hpp"

namespace Light
{
	class ResourceManager
	{
	public:
		ResourceManager() {};
		~ResourceManager() {};

		template<class T, typename Func, typename... Args>
		std::shared_ptr<T> get(const std::string& handle, Func&& func, Args... args)
		{
			static_assert(std::is_same_v<std::invoke_result_t<decltype(func), Args...>, std::unique_ptr<T> >, "Func must return unique_ptr<T>");

			if (m_caches.size() <= static_cast<size_t>(T::getStaticType()))
			{
				m_caches.resize(T::getStaticType() + 1);
				m_caches[T::getStaticType()] = std::make_unique<ResourceCache<T>>();
			}

			return m_caches[T::getStaticType()]->get<T>(handle, func, args...);
		}

		template<class T>
		std::shared_ptr<T> get(const std::string& handle)
		{
			if (m_caches.size() <= static_cast<size_t>(T::getStaticType()))
			{
				m_caches.resize(T::getStaticType() + 1);
				m_caches[T::getStaticType()] = std::make_unique<ResourceCache<T>>();
			}

			return m_caches[T::getStaticType()]->get<T>(handle);
		}

	private:
		std::vector<std::unique_ptr<ResourceCacheBase>> m_caches;
	};
}

#endif // __RESOURCEMANAGER_H__
