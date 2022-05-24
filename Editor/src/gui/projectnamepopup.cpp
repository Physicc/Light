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
		if(m_isOpen)
		{
			ImGui::OpenPopup("Open Project##Project");
		}

		if(ImGui::BeginPopupModal("Open Project##Project", &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, m_projectPath.string().c_str(), sizeof(buffer)-1);
			if(ImGui::InputText("##ProjectPath", buffer, sizeof(buffer)-1))
			{
				m_projectPath = buffer;
			}

			ImGui::SameLine();

			if (ImGui::Button("..."))
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
				ImGui::NewLine();
			}

			if(ImGui::Button("OK"))
			{
				if(m_callback)
				{
					m_callback(m_projectPath.string());
				}
				ImGui::CloseCurrentPopup();
				m_isOpen = false;
			}
			if (!isDir)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if(ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				m_isOpen = false;
			}


			ImGui::EndPopup();
		}
	}

	void ProjectNamePopup::openPopup()
	{
		m_isOpen = true;
	}
}
