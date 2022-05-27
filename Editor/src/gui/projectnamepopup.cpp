#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include "gui/projectnamepopup.hpp"

#include "core/logging.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "core/application.hpp"

#include "nfd.hpp"


namespace Light
{
	void ProjectNamePopup::onImguiRender()
	{
		if (m_toOpen)
		{
			NFD::UniquePath outpath;

			auto result = NFD::PickFolder(outpath, nullptr, "Select Project Folder");
			if (result == NFD_OKAY)
			{
				m_projectPath = outpath.get();
				m_callback(m_projectPath.string());
			}
			else if (result == NFD_CANCEL)
			{
			}
			else
			{
				LIGHT_CORE_ERROR("NFD Error: {0}", NFD::GetError());
			}

			m_toOpen = false;
		}
		// if(m_toOpen)
		// {
		// 	ImGui::OpenPopup("Open Project##Project");
		// 	if(Application::get().getConfig().Has("project_path"))
		// 	{
		// 		m_projectPath = Application::get().getConfig().GetString("project_path");
		// 	}
		// 	m_toOpen = false;
		// }

		// bool p_open = true;
		// if(ImGui::BeginPopupModal("Open Project##Project", &p_open, ImGuiWindowFlags_AlwaysAutoResize))
		// {
		// 	char buffer[256];
		// 	memset(buffer, 0, sizeof(buffer));
		// 	std::strncpy(buffer, m_projectPath.string().c_str(), sizeof(buffer)-1);
		// 	if(ImGui::InputText("##ProjectPath", buffer, sizeof(buffer)-1))
		// 	{
		// 		m_projectPath = buffer;
		// 	}

		// 	ImGui::SameLine();

		// 	if (ImGui::Button("..."))
		// 	{
		// 		NFD::UniquePath outpath;

		// 		auto result = NFD::PickFolder(outpath, nullptr, "Select Project Folder");
		// 		if (result == NFD_OKAY)
		// 		{
		// 			m_projectPath = outpath.get();
		// 		}
		// 		else if (result == NFD_CANCEL)
		// 		{
		// 		}
		// 		else
		// 		{
		// 			LIGHT_CORE_ERROR("NFD Error: {0}", NFD::GetError());
		// 		}
		// 	}

		// 	bool isDir = std::filesystem::is_directory(m_projectPath);

		// 	if (!isDir)
		// 	{
		// 		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		// 		ImGui::Text("The path has to be an existing directory");
		// 		ImGui::PopStyleColor();

		// 		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        // 		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		// 	}
		// 	else
		// 	{
		// 		ImGui::NewLine();
		// 	}

		// 	if(ImGui::Button("OK"))
		// 	{
		// 		if(m_callback)
		// 		{
		// 			m_callback(m_projectPath.string());
		// 		}
		// 		ImGui::CloseCurrentPopup();
		// 	}
		// 	if (!isDir)
		// 	{
		// 		ImGui::PopItemFlag();
		// 		ImGui::PopStyleVar();
		// 	}

		// 	ImGui::SetItemDefaultFocus();
		// 	ImGui::SameLine();
		// 	if(ImGui::Button("Cancel"))
		// 	{
		// 		ImGui::CloseCurrentPopup();
		// 	}


		// 	ImGui::EndPopup();
		// }
	}

	void ProjectNamePopup::openPopup()
	{
		m_toOpen = true;
	}
}
