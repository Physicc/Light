#include "gui/scenepanel.hpp"

#include "imgui.h"

namespace Light
{
	void ScenePanel::onImguiRender() 
	{
		ImGui::Begin("Scene Properties");

		m_context->m_registry.each([&](auto entityHandle)
		{
			Entity entity = {entityHandle, m_context.get()};
			drawSceneNode(entity);
		});

		ImGui::End();
	}

	void ScenePanel::drawSceneNode(Entity entity) 
	{
		auto& tag = entity.getComponent<TagComponent>().tag;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0);
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
}