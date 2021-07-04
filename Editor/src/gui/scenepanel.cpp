#include "gui/scenepanel.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace Light
{
	void ScenePanel::onImguiRender() 
	{
		ImGui::Begin("Scene");

		m_context->m_registry.each([&](auto entityHandle)
		{
			Entity entity = {entityHandle, m_context.get()};
			drawSceneNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");

		if(m_selectionContext)
			drawAllComponents(m_selectionContext);

		ImGui::End();
	}

	void ScenePanel::drawSceneNode(Entity entity) 
	{
		auto& tag = entity.getComponent<TagComponent>().tag;
		
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx(entity.getUUID().c_str(), flags, tag.c_str());
		
		if(ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		if(opened)
		{
			ImGui::TreePop();
		}
	}

	template<typename T, typename UIFunction>
	void drawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		if(entity.hasComponent<T>())
		{
			auto& component = entity.getComponent<T>();

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx(component.uuid.c_str(), flags, name.c_str());

			if(open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
		}
	}

	static bool dragVec3(std::string label, glm::vec3& vec3,
		float v_speed = 1.0f, float resetValue = 0.0f,
		float minLabelWidth = 100.0f, float minSliderWidth = 200.0f, const char* fmt = "%.3f",
		float v_min = 0.0f, float v_max = 0.0f)
	{
		bool ret = false;
		ImGui::PushID(label.c_str());

		float itemWidth = ImGui::GetContentRegionAvail().x;
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, glm::max(itemWidth/3, minLabelWidth));
		ImGui::Text(label.c_str());
		if(ImGui::BeginPopupContextItem("reset"))
		{
			if(ImGui::MenuItem("Reset Values"))
			{
				vec3 = glm::vec3(resetValue);
				ret = true;
			}
			ImGui::EndPopup();
		}
		ImGui::NextColumn();

		float fullWidth = glm::max(ImGui::GetContentRegionAvail().x, minSliderWidth);
		float offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
		if(offset > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
		float width = fullWidth / 3.0f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::SetNextItemWidth(lineHeight + 2.0f);
		ret |= ImGui::DragFloat("##x", &vec3.x, v_speed, v_min, v_max, "X", ImGuiSliderFlags_NoInput);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(width - lineHeight - 2.0f);
		ret |= ImGui::InputFloat("##xedit", &vec3.x, 0.0f, 0.0f, fmt);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::SetNextItemWidth(lineHeight + 2.0f);
		ret |= ImGui::DragFloat("##y", &vec3.y, v_speed, v_min, v_max, "Y", ImGuiSliderFlags_NoInput);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(width - lineHeight - 2.0f);
		ret |= ImGui::InputFloat("##yedit", &vec3.y, 0.0f, 0.0f, fmt);
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::SetNextItemWidth(lineHeight + 2.0f);
		ret |= ImGui::DragFloat("##z", &vec3.z, v_speed, v_min, v_max, "Z", ImGuiSliderFlags_NoInput);
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::SetNextItemWidth(fullWidth - width * 2 - lineHeight - 2.0f);
		ret |= ImGui::InputFloat("##zedit", &vec3.z, 0.0f, 0.0f, fmt);
		
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return ret;
	}

	void ScenePanel::drawAllComponents(Entity entity) 
	{
		if(entity.hasComponent<TagComponent>())
		{
			auto& tag = entity.getComponent<TagComponent>().tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, tag.c_str(), sizeof(buffer)-1);
			if(ImGui::InputText("##Tag", buffer, sizeof(buffer)-1))
			{
				tag = std::string(buffer);
			}
		}

		drawComponent<TransformComponent>("Transform", entity, [](auto& component){
			dragVec3("Position", component.position, 0.1f);
			glm::vec3 rotationDeg = glm::degrees(component.rotation);
			if(dragVec3("Rotation", rotationDeg, 5.0f))
				component.rotation = glm::radians(rotationDeg);
			dragVec3("Scale", component.scale, 0.1f, 1.0f);
		});
	}
}