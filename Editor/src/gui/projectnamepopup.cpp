#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include "gui/projectnamepopup.hpp"

#include "core/logging.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "core/application.hpp"

#include "nfd.hpp"

#include <thread>
#include <future>
#include <chrono>

namespace Light
{
	void ProjectNamePopup::onImguiRender()
	{
		static NFD::UniquePath outpath;
		static std::future<nfdresult_t> result;
		if (m_toOpen && !m_isOpen)
		{
			result = std::async(std::launch::async,
								[](NFD::UniquePath& outpath)
								{
									// Init and Uninit in the same thread as Dialog creation
									NFD::Init();
									auto ret = NFD::PickFolder(outpath, nullptr, "Open Project");
									NFD::Quit();
									return ret;
								},
								std::reference_wrapper(outpath));
			m_toOpen = false;
			m_isOpen = true;
		}

		if(m_isOpen)
		{
			auto status = result.wait_for(std::chrono::milliseconds(0));

			if (status == std::future_status::ready)
			{
				auto res = result.get();
				if (res == NFD_OKAY)
				{
					m_projectPath = outpath.get();
					m_callback(m_projectPath.string());
				}
				else if (res == NFD_CANCEL)
				{
				}
				else
				{
					LIGHT_CORE_ERROR("NFD Error: {0}", NFD::GetError());
				}
				m_isOpen = false;
			}
		}
	}

	void ProjectNamePopup::openPopup()
	{
		m_toOpen = true;
	}
}
