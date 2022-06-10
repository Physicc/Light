#ifndef __ASSETBROWSER_H__
#define __ASSETBROWSER_H__

#include <filesystem>
#include <set>

#include "core/timestep.hpp"
#include "events/event.hpp"
#include "events/keyevent.hpp"

namespace Light
{
	class AssetBrowser
	{
	public:
		AssetBrowser() = default;
		~AssetBrowser() = default;

		void onImguiRender();

		void onEvent(Event& e);
		void onUpdate(Timestep ts);

	private:

		bool onKeyPressed(KeyPressedEvent& e);

		void drawDir(std::filesystem::path dir);
		void drawFile(std::filesystem::path file);

		void drawNewFolder();

		std::filesystem::path m_projectDir;
		std::set<std::filesystem::path> m_selectedPaths;
		bool m_toOpenDeletePopup = false;
		bool m_toDeleteSelected = false;
		std::filesystem::path m_toCreateNewFolderAt;
		bool m_newFolderInputTextHover = false;
	};
}

#endif // __ASSETBROWSER_H__
