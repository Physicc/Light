
#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include "gui/scenepanel.hpp"


#include "imgui.h"
#include "imgui_internal.h"

#include <type_traits>


namespace Light
{
	void ScenePanel::onImguiRender()
	{
		ImGui::Begin("Scene");

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("Add Entity"))
			{
				if (ImGui::MenuItem("Empty Entity"))
				{
					m_context->addEntity("New Entity");
				}

				if (ImGui::MenuItem("Light"))
				{
					auto entity = m_context->addEntity("New Entity");
					entity.addComponent<LightComponent>();
				}

				if (ImGui::BeginMenu("Primitive"))
				{
					if (ImGui::MenuItem("Cube"))
					{
						auto entity = m_context->addEntity("Cube");
						entity.addComponent<MeshComponent>(m_meshLibrary->get("Cube"));
						entity.addComponent<MeshRendererComponent>("assets/shaders/phong.glsl");
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		m_context->m_registry.each([&](auto entityHandle)
		{
			Entity entity = {entityHandle, m_context.get()};
			drawSceneNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");

		if(m_selectionContext && ImGui::BeginPopupContextWindow())
		{
			if (ImGui::BeginMenu("Add Component"))
			{
				if(ImGui::MenuItem("Mesh", nullptr, false, !m_selectionContext.hasComponent<MeshComponent>()))
				{
					m_selectionContext.addComponent<MeshComponent>(m_meshLibrary->getMeshMap().begin()->second);
				}
				if(ImGui::MenuItem("Mesh Renderer", nullptr, false, !m_selectionContext.hasComponent<MeshRendererComponent>()))
				{
					m_selectionContext.addComponent<MeshRendererComponent>("assets/shaders/phong.glsl");
				}
				if(ImGui::MenuItem("Light", nullptr, false, !m_selectionContext.hasComponent<LightComponent>()))
				{
					m_selectionContext.addComponent<LightComponent>();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		if(m_selectionContext)
			drawAllComponents(m_selectionContext);

		ImGui::End();
	}

	void ScenePanel::drawSceneNode(Entity entity)
	{
		auto& tag = entity.getComponent<TagComponent>().tag;

		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx(entity.getUUID().c_str(), flags, "%s", tag.c_str()); // Use %s as argument to avoid format string vulnerability
		bool toRemove = false;
		if(ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		if(ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Entity"))
			{
				toRemove = true;
			}
			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGui::TreePop();
		}

		if(toRemove)
		{
			m_context->removeEntity(entity);
			m_selectionContext = {};
		}
	}

	template<typename T, typename UIFunction>
	void drawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		constexpr bool removable = !std::is_same_v<T, TransformComponent>;

		bool toRemove = false;
		if(entity.hasComponent<T>())
		{
			auto& component = entity.getComponent<T>();

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx(component.uuid.c_str(), flags, "%s", name.c_str());
			if constexpr(removable)
			{
				if (ImGui::BeginPopupContextItem())
				{
					if(ImGui::MenuItem("Remove Component"))
					{
						toRemove = true;
					}
					ImGui::EndPopup();
				}
			}

			if(open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
		}

		if(toRemove)
			entity.removeComponent<T>();
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
		ImGui::Text("%s", label.c_str());
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
			std::strncpy(buffer, tag.c_str(), sizeof(buffer)-1);
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

		drawComponent<MeshComponent>("Mesh", entity, [this](auto& component){
			// float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float itemWidth = ImGui::GetContentRegionAvail().x;

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));

			ImGui::Text("Mesh");

			ImGui::NextColumn();

			float fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
			float offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
			if(offset > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

			ImGui::SetNextItemWidth(fullWidth);

			auto meshMap = m_meshLibrary->getMeshMap();
			auto current = meshMap.begin();

			for (auto it = meshMap.begin(); it != meshMap.end(); it++)
			{
				if(it->second == component.mesh)
				{
					current = it;
					break;
				}
			}

			if (ImGui::BeginCombo("##mesh", current->first.c_str(), 0))
			{
				for(auto it = meshMap.begin(); it != meshMap.end(); it++)
				{
					const bool selected = current == it;
					if(ImGui::Selectable(it->first.c_str(), selected))
					{
						current = it;
						component.mesh = it->second;
					}

					if(selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Columns(1);
		});

		drawComponent<MeshRendererComponent>("Mesh Renderer", entity, [](auto&){
			ImGui::Text("Phong");
		});

		drawComponent<LightComponent>("Light", entity, [](auto& component){
			// float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			float itemWidth = ImGui::GetContentRegionAvail().x;

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));

			ImGui::Text("Light Color");

			ImGui::NextColumn();

			float fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
			float offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
			if(offset > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

			ImGui::SetNextItemWidth(fullWidth);

			ImGui::ColorEdit3("##color", &component.m_lightColor[0]);

			ImGui::Columns(1);

			ImGui::Columns(2, NULL, false);
			ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));

			ImGui::Text("Light Type");

			ImGui::NextColumn();

			fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
			offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
			if(offset > 0.0f)
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

			ImGui::SetNextItemWidth(fullWidth);

			auto type = component.m_lightType;

			if (ImGui::BeginCombo("##lighttype", LightTypeStrings[(int)type].c_str(), 0))
			{
				for(int it = 0; it < (int)LightType::NumLightTypes; it++)
				{
					const bool selected = (int)type == it;
					if(ImGui::Selectable(LightTypeStrings[it].c_str(), selected))
					{
						type = static_cast<LightType>(it);
						component.m_lightType = static_cast<LightType>(it);
					}

					if(selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Columns(1);

			if(type == LightType::Spot)
			{	
				

				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				const char* fmt = "%.3f";
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));
				ImGui::Text("Inner Cutoff");
				ImGui::NextColumn();

				fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
				offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
				if(offset > 0.0f)
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

				ImGui::SetNextItemWidth(fullWidth);
				float width = fullWidth / 3.0f;
				ImGui::SetNextItemWidth(width - lineHeight - 2.0f);
				if(ImGui::DragFloat("##innerRad", &component.m_inner, 1.0f, 0.0f, 90.0f, fmt))
				{
					component.m_inner = glm::clamp(component.m_inner, 0.0f, 90.0f);
					component.m_outer = glm::clamp(component.m_outer, component.m_inner, 90.0f);
				}


				ImGui::Columns(1);

				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));
				ImGui::Text("Outer Cutoff");
				ImGui::NextColumn();

				fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
				offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
				if(offset > 0.0f)
				 		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
				ImGui::SetNextItemWidth(fullWidth);
				width = fullWidth / 3.0f;
				ImGui::SetNextItemWidth(width - lineHeight - 2.0f);
				if(ImGui::DragFloat("##outerRad", &component.m_outer, 1.0f, 0.0f, 90.0f, fmt))
				{	
					component.m_outer = glm::clamp(component.m_outer, 0.0f, 90.0f);
					component.m_inner = glm::clamp(component.m_inner, 0.0f, component.m_outer);
				}
				

				ImGui::Columns(1);

			}

			if(type == LightType::Spot || type == LightType::Point)
			{	
				
				component.m_range = (component.m_range>0.01f)?component.m_range:1;
				
			
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				const char* fmt = "%.3f";
				ImGui::Columns(2, NULL, false);
				ImGui::SetColumnWidth(0, glm::max(itemWidth/3, 100.0f));
				ImGui::Text("Range");
				ImGui::NextColumn();

				fullWidth = glm::max(ImGui::GetContentRegionAvail().x, 200.0f);
				offset = glm::max(0.0f, ImGui::GetContentRegionAvail().x - fullWidth);
				if(offset > 0.0f)
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);

				ImGui::SetNextItemWidth(fullWidth);
				float width = fullWidth / 3.0f;
				ImGui::SetNextItemWidth(width - lineHeight - 2.0f);
				if(ImGui::DragFloat("##range", &component.m_range, 1.0f, 0.1f, FLT_MAX, fmt))
				{
					component.m_range = (component.m_range < 0.1f) ? 0.1f : component.m_range;
				}


				ImGui::Columns(1);

			}
			
		});
	}
}
