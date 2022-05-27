#include "gui/assetbrowser.hpp"

#include "core/application.hpp"
#include "input/keycodes.hpp"
#include "input/mousecodes.hpp"

#include "imgui.h"
#include <filesystem>

#include "core/logging.hpp"
#include "core/base.hpp"

namespace Light
{
	void AssetBrowser::drawDir(std::filesystem::path dir)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		bool selected = m_selectedPaths.find(dir) != m_selectedPaths.end();
		node_flags |=( selected ? ImGuiTreeNodeFlags_Selected : 0 );
		std::string dirName = dir.filename().string();
		bool node_open = ImGui::TreeNodeEx(dir.string().c_str(), node_flags, "%s", dirName.c_str());
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
			ImGui::TreePop();
		}
	}

	void AssetBrowser::drawFile(std::filesystem::path file)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
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

		if (ImGui::IsMouseDown(LIGHT_MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() )
		{
			m_selectedPaths.clear();
		}

		if (!m_projectDir.empty())
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

		ImGui::End();

		if (m_toOpenDeletePopup)
		{
			ImGui::OpenPopup("Delete?");
			m_toOpenDeletePopup = false;
		}

		bool p_open = true;
		if(ImGui::BeginPopupModal("Delete?", &p_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete the selected files?");
			ImGui::Text("This action cannot be undone.");
			if (ImGui::Button("Yes"))
			{
				m_toDeleteSelected = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				ImGui::CloseCurrentPopup();
			}
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
