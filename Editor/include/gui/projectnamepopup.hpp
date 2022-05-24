#ifndef __PROJECTNAMEPOPUP_H__
#define __PROJECTNAMEPOPUP_H__

#include <filesystem>

namespace Light
{
	class ProjectNamePopup
	{
	public:
		ProjectNamePopup() = default;
		~ProjectNamePopup() = default;

		bool onImguiRender();

	private:
		std::filesystem::path m_projectPath;
	};
}

#endif // __PROJECTNAMEPOPUP_H__
