#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/base.hpp"
#include "rendering/vertexarray.hpp"
#include "rendering/camera.hpp"
#include "rendering/shader.hpp"

namespace Light
{
	class Renderer
	{
	public:
		static void init();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(Camera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform = glm::mat4(1.0f));
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* sceneData;
	};
	
}

#endif // __RENDERER_H__