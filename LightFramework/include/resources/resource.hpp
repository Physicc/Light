#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "glm/glm.hpp"

#include "light/rendering/buffer.hpp"

#include "core/logging.hpp"

// namespace Light
// {
// 	template <typename T>
// 	class Resource
// 	{
// 	public:
// 		Resource(std::string location, std::string name = "") : m_location(location), m_name(name)
// 		{
// 			if(m_location.length != 0 && m_name.length() == 0)
// 			{
// 				m_name = location;
// 			}
// 		}

// 		template <typename... Args>
// 		void create(Args&&... args)
// 		{
// 			if(m_location.length() > 0)
// 			{
// 				LIGHT_CORE_WARN("Will not create resource {0} which can be loaded from disk", m_name);
// 				return;
// 			}

// 			m_resourcePointer = std::make_shared<T>(std::forward<Args>(args)...);
// 		}

// 		void load()
// 		{
// 			if(m_location.length() == 0)
// 			{
// 				LIGHT_CORE_ERROR("Cannot load resource {0} from disk without disk location", m_name);
// 				return;
// 			}
// 			else
// 			{
// 				m_resourcePointer = std::shared_ptr<T>(T::loadFromFile(m_filename));
// 			}

// 			m_loaded = true;
// 		}

// 		void unload()
// 		{
// 			if(m_location.length() == 0)
// 			{
// 				LIGHT_CORE_WARN("Won't be able to reload resource {0} from disk without disk location", m_name);
// 				m_loaded = false;
// 				return;
// 			}

// 			m_loaded = false;
// 		}

// 		inline std::shared_ptr<T> operator->() { return m_resourcePointer; }

// 	private:
// 		std::shared_ptr<T> m_resourcePointer;

// 		std::string m_name;
// 		std::string m_location;

// 		bool m_loaded;
// 	};
// }




#endif // __RESOURCE_H__

