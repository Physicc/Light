#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include "gui/assetbrowser.hpp"

#include "core/application.hpp"
#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include <filesystem>

#include "core/logging.hpp"
#include "core/base.hpp"

namespace ImGui
{
	// See here: https://github.com/ocornut/imgui/issues/589
	static bool IsItemActivePreviousFrame()
	{
		ImGuiContext& g = *GImGui;
		if (g.ActiveIdPreviousFrame)
			return g.ActiveIdPreviousFrame== GImGui->CurrentWindow->DC.LastItemId;
		return false;
	}
}

namespace Light
{
	void AssetBrowser::drawDir(std::filesystem::path dir)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding;
		bool selected = m_selectedPaths.find(dir) != m_selectedPaths.end();
		node_flags |= ( selected ? ImGuiTreeNodeFlags_Selected : 0 );

		std::string dirName = dir.filename().string();

		if (m_toCreateNewFolderAt == dir)
			ImGui::SetNextItemOpen(true);

		bool node_open = ImGui::TreeNodeEx(dir.string().c_str(), node_flags, "%s", dirName.c_str());

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("New Folder"))
			{
				m_toCreateNewFolderAt = dir;
			}
			if (ImGui::MenuItem("Delete"))
			{
				if (m_selectedPaths.empty())
					m_selectedPaths.insert(dir);
				m_toOpenDeletePopup = true;
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
		{
			if(ImGui::IsKeyDown(LIGHT_KEY_LEFT_CONTROL))
			{
				if (selected)
					m_selectedPaths.erase(dir);
				else
					m_selectedPaths.insert(dir);
			}
			else
			{
				m_selectedPaths.clear();
				m_selectedPaths.insert(dir);
			}
		}
		if (node_open)
		{
			for (auto& p : std::filesystem::directory_iterator(dir))
			{
				if (p.is_directory())
				{
					drawDir(p.path());
				}
				else
				{
					drawFile(p.path());
				}
			}

			if (m_toCreateNewFolderAt == dir)
			{
				drawNewFolder();
			}

			ImGui::TreePop();
		}

	}

	void AssetBrowser::drawFile(std::filesystem::path file)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_FramePadding;
		bool selected = m_selectedPaths.find(file) != m_selectedPaths.end();
		node_flags |=( selected ? ImGuiTreeNodeFlags_Selected : 0 );
		std::string fileName = file.filename().string();
		ImGui::TreeNodeEx(file.string().c_str(), node_flags, "%s", fileName.c_str());
		if (ImGui::IsItemClicked())
		{
			if(ImGui::IsKeyDown(LIGHT_KEY_LEFT_CONTROL))
			{
				if (selected)
					m_selectedPaths.erase(file);
				else
					m_selectedPaths.insert(file);
			}
			else
			{
				m_selectedPaths.clear();
				m_selectedPaths.insert(file);
			}
		}
	}

	void AssetBrowser::drawNewFolder()
	{
		ImGuiTreeNodeFlags node_flags =   ImGuiTreeNodeFlags_SpanFullWidth
										| ImGuiTreeNodeFlags_NoTreePushOnOpen
										| ImGuiTreeNodeFlags_FramePadding;
		ImGui::TreeNodeEx("##NewFolder", node_flags);
		ImGui::SetItemAllowOverlap();
		ImGui::SameLine();
		static std::string newFolderName;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, newFolderName.c_str());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = ImGui::InputText("##NewFolderName", buffer, sizeof(buffer) - 1);
		ImGui::SetKeyboardFocusHere(0);

		bool isEnterPressed = ImGui::IsItemActivePreviousFrame() && !ImGui::IsItemActive() && ImGui::IsKeyPressed(LIGHT_KEY_ENTER);
		bool isEscPressed =  ImGui::IsItemActivePreviousFrame() && !ImGui::IsItemActive() && ImGui::IsKeyPressed(LIGHT_KEY_ESCAPE);
		m_newFolderInputTextHover = ImGui::IsItemHovered();

		if (modified)
		{
			newFolderName = buffer;
		}

		if (isEnterPressed || isEscPressed)
		{
			if (!newFolderName.empty())
			{
				std::filesystem::create_directory(m_toCreateNewFolderAt / newFolderName);
			}
			m_toCreateNewFolderAt.clear();
			newFolderName.clear();
		}

	}

	void AssetBrowser::onImguiRender()
	{
		if (Application::get().getConfig().Has("project_path"))
		{
			m_projectDir = Application::get().getConfig().GetString("project_path");
		}
		else
		{
			m_projectDir = std::filesystem::path();
		}

		ImGui::Begin("Asset Browser");

		if(ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("New Folder"))
			{
				m_toCreateNewFolderAt = m_projectDir;
			}
			if (!m_selectedPaths.empty() && ImGui::MenuItem("Delete"))
			{
				m_toOpenDeletePopup = true;
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDown(LIGHT_MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
		{
			m_selectedPaths.clear();
		}
		if ((ImGui::IsMouseDown(LIGHT_MOUSE_BUTTON_LEFT) || ImGui::IsMouseDown(LIGHT_MOUSE_BUTTON_RIGHT)) && !m_newFolderInputTextHover)
		{
			m_toCreateNewFolderAt.clear();
		}

		if (!m_projectDir.empty())
		{
			for (auto& p : std::filesystem::directory_iterator(m_projectDir))
			{
				if (p.is_directory())
				{
					drawDir(p.path());
				}
				else
				{
					drawFile(p.path());
				}
			}

			if (m_toCreateNewFolderAt == m_projectDir)
			{
				drawNewFolder();
			}
		}

		ImGui::End();

		if (m_toOpenDeletePopup && !m_selectedPaths.empty())
		{
			ImGui::OpenPopup("Delete?");
			m_toOpenDeletePopup = false;
		}

		bool p_open = true;
		if(ImGui::BeginPopupModal("Delete?", &p_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Set maximum text widget size
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("Are you sure you want to permanently delete the following files/folders and their contents?");
			ImGui::Separator();
			for (auto& p : m_selectedPaths)
			{
				ImGui::Text("%s", p.filename().string().c_str());
			}
			ImGui::Separator();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text("This action cannot be undone.");
			ImGui::PopStyleColor();

			// Center the two buttons
			ImGuiStyle& style = ImGui::GetStyle();
			float width = 0.0f;
			width += ImGui::CalcTextSize("Delete").x + style.FramePadding.x * 2.0f;
			width += style.ItemSpacing.x;
			width += ImGui::CalcTextSize("Cancel").x + style.FramePadding.x * 2.0f;
			float availWidth = ImGui::GetContentRegionAvailWidth();
			float offset = (availWidth - width) * 0.5f;
			ImGui::SetCursorPosX( ImGui::GetCursorPosX() + offset);

			if (ImGui::Button("Delete"))
			{
				ImGui::SetItemDefaultFocus();
				m_toDeleteSelected = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopTextWrapPos();
			ImGui::EndPopup();
		}
	}

	bool AssetBrowser::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getKeycode() == LIGHT_KEY_DELETE)
		{
			m_toOpenDeletePopup = true;
			return true;
		}

		return false;
	}

	void AssetBrowser::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(AssetBrowser::onKeyPressed));
	}

	void AssetBrowser::onUpdate(Timestep)
	{
		if (m_toDeleteSelected)
		{
			for (auto& p : m_selectedPaths)
			{
				// If the file is a directory, delete it recursively
				if (std::filesystem::is_directory(p))
				{
					std::filesystem::remove_all(p);
				}
				else
				{
					std::filesystem::remove(p);
				}
			}
			m_toDeleteSelected = false;
		}
	}

} // namespace Light
