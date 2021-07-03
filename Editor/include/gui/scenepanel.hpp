#ifndef __SCENEPANEL_H__
#define __SCENEPANEL_H__

#include "light.hpp"

namespace Light
{
	class ScenePanel
	{
	public:
		ScenePanel() = default;
		ScenePanel(std::shared_ptr<Light::Scene> context) : m_context(context) {}

		inline void setContext(std::shared_ptr<Light::Scene> context) { m_context = context; }

		void onImguiRender();
	private:

		void drawSceneNode(Entity entity);

		std::shared_ptr<Light::Scene> m_context; 
		Entity m_selectionContext;
	};

}

#endif // __SCENEPANEL_H__