#include "Editor.hpp"
#include "core/entrypoint.hpp"

#include <filesystem>

namespace Light
{
	Application* createApplication()
	{
		return new Editor();
	}

	void Editor::loadConfig()
	{
		// Check if m_configPath exists
		if (std::filesystem::exists(m_configPath))
		{
			getConfig().Load(m_configPath);
		}
	}
}
