#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "core/base.hpp"

#include "light/rendering/vertexarray.hpp"
#include "light/rendering/camera.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/lights.hpp"

namespace Light
{
	class Renderer
	{
	public:
		static void init();
		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(Camera& camera, glm::mat4 camera_view);
		static void endScene();

		static void submitLight(const std::vector<PointLight>& lights);
		static void submitLight(const std::vector<SpotLight>& lights);
		static void submitLight(const std::vector<DirectionalLight>& lights);
		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform = glm::mat4(1.0f));
		static void submitID(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao, glm::mat4 transform = glm::mat4(1.0f), int id = -1);
		static void submitSkybox(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vao);

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
			glm::mat4 viewProjectionSkyboxMatrix;
			glm::vec3 cameraPosition;
			std::vector<PointLight> pointLights;
			std::vector<SpotLight> spotLights;
			std::vector<DirectionalLight> directionalLights;
		};

		static SceneData* s_sceneData;
	};

}

#endif // __RENDERER_H__