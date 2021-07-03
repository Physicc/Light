#include "gui/scenepanel.hpp"

#include "imgui.h"

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
			ImGui::DragFloat3("Position", &(component.position.x), 0.5f);
		});
	}
}