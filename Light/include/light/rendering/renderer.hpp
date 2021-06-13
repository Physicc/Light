#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/base.hpp"

#include "light/rendering/vertexarray.hpp"
#include "light/rendering/camera.hpp"
#include "light/rendering/shader.hpp"
namespace Light
{
	class Renderer
	{
	public:
		static void init();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(Camera& camera, glm::vec3 lightPos);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform = glm::mat4(1.0f));
		static void submitSkybox(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao);
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
			glm::vec3 lightPos;
			glm::mat4 viewProjectionSkyboxMatrix;
		};

		static SceneData* s_sceneData;
	};
	
}

#endif // __RENDERER_H__