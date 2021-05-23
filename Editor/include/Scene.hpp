#ifndef __SCENE_H__
#define __SCENE_H__

#include "light.hpp"
#include "Objects.hpp"

class Scene 
{
	public:
    
    Scene();
	void onUpdate(Light::Timestep ts);
	void render();
	void onEvent(Light::Event& e);
	void setViewportSize(int &&,int &&);
	void onWindowResize(Light::WindowResizeEvent& );
    float getlightPosx();
	void addgameObject(glm::vec3,glm::vec3,glm::vec3);

	private:
	
	Light::EditorCamera m_camera;
	std::vector<Cube> m_gameObjects;
	Cube m_floor;
	Skybox m_skybox;
	glm::vec3 m_lightPos;
	

};
#endif
