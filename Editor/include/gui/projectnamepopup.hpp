#ifndef __PROJECTNAMEPOPUP_H__
#define __PROJECTNAMEPOPUP_H__

#include <filesystem>
#include <functional>

namespace Light
{
	class ProjectNamePopup
	{
	public:
		ProjectNamePopup() = default;
		~ProjectNamePopup() = default;

		void onImguiRender();
		void openPopup();

		void setInputCallback(std::function<void(const std::string&)> callback) {m_callback = callback;}

	private:
		std::filesystem::path m_projectPath;
		bool m_toOpen = false;
		bool m_isOpen = false;
		std::function<void(const std::string&)> m_callback;
	};
}

#endif // __PROJECTNAMEPOPUP_H__
