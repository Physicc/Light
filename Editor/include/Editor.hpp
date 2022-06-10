#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "layers/editorlayer.hpp"
#include "core/application.hpp"

namespace Light{
	class Editor : public Application
	{
	public:
		Editor()
		{
			loadConfig();
			pushLayer(new EditorLayer());
		}
		~Editor() = default;

		inline virtual std::filesystem::path getConfigPath() override
		{
			return m_configPath;
		}

	private:
		void loadConfig();
		std::string m_configPath = "editorconfig.json";
	};
}

#endif // __EDITOR_H__
