#ifndef __SCENERENDERER_H__
#define __SCENERENDERER_H__

#include <memory>

#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "rendering/editorcamera.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/vertexarray.hpp"
#include "light/rendering/framebuffer.hpp"

namespace Light {

	/*
	 * 	SceneRenderer is responsible for rendering the scene.
	 *  Usage:
	 * 	~~~~~~~~~~~~~~~{.cpp}
	 * 		SceneRenderer::setTargetFramebuffer(framebuffer); // To set the target framebuffer
	 * 		SceneRenderer::renderEditor(scene, camera); // To render the scene
	 * 		SceneRenderer::renderOutline(scene, entity); // To render the outline for an entity
	 * 	~~~~~~~~~~~~~~~
	 *
	 */
	class SceneRenderer {
	public:
		SceneRenderer();
		~SceneRenderer() {};

		void renderEditor(std::shared_ptr<Scene> scene, EditorCamera &camera);
		void renderOutline(std::shared_ptr<Scene> scene, Entity entity);

		void onViewportResize(int width, int height);

		/*
		 *  The framebuffer should have the format of {RGBA8, RED_INTEGER, DEPTH24_STENCIL8}
		 *
		 * 	@param framebuffer - The framebuffer to render to
		 */
		void setTargetFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

	private:
		std::shared_ptr<Light::Shader> m_skybox_shader;
		std::shared_ptr<Light::Shader> m_outline_shader;
		std::shared_ptr<Light::Shader> m_outline_temp_shader;
		std::shared_ptr<Light::VertexArray> m_skybox_mesh;
		std::shared_ptr<Light::VertexArray> m_outline_mesh;

		std::shared_ptr<Light::Framebuffer> m_framebuffer;
		std::shared_ptr<Light::Framebuffer> m_outlineFramebuffer;
		std::shared_ptr<Light::Framebuffer> m_tempFramebuffer;
		//TODO: #63 Directly use texture instead of a dummy framebuffer
	};
}

#endif // __SCENERENDERER_H__
