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
	bool ProjectNamePopup::onImguiRender()
	{

		bool used = false;
		if(!Application::get().getConfig().KeyExists("project_path"))
		{
			ImGui::OpenPopup("Project");
		}

		if(ImGui::BeginPopupModal("Project"))
		{
			ImGui::Text("Please select the project path");
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, m_projectPath.string().c_str(), sizeof(buffer)-1);
			if(ImGui::InputText("##ProjectPath", buffer, sizeof(buffer)-1))
			{
				m_projectPath = buffer;
			}
			if (ImGui::Button("...", ImVec2(120, 0)))
			{
				NFD::UniquePath outpath;

				auto result = NFD::PickFolder(outpath);
				if (result == NFD_OKAY)
				{
					m_projectPath = outpath.get();
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
					LIGHT_CORE_ERROR("NFD Error: {0}", NFD::GetError());
				}
			}

			bool isDir = std::filesystem::is_directory(m_projectPath);

			if (!isDir)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::Text("The path has to be an existing directory");
				ImGui::PopStyleColor();

				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			else
			{
				ImGui::Text("");
			}

			if(ImGui::Button("OK", ImVec2(120, 0)))
			{
				Application::get().getConfig().SetString("project_path", std::filesystem::absolute(m_projectPath).string());
				ImGui::CloseCurrentPopup();
				used = true;
			}

			if (!isDir)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			ImGui::EndPopup();
		}

		return used;
	}
}
