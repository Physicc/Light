#include "light.hpp"
#include "Scene.hpp"

Scene::Scene()
	: 
	     
		m_camera(45.0f, 1.6f/0.9f, 0.001f, 100.0f),
		m_lightPos(-1,2,1.5),
		m_floor(glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(2,0.1,2))
{
	     

}
void Scene::setViewportSize(int && width,int && height){
    
    m_camera.setViewportSize(width, height);

}
void Scene::onUpdate(Light::Timestep ts)
{
	    m_camera.onUpdate(ts);
		for(int i=0;i<m_gameObjects.size();i++){
		m_gameObjects[i].onUpdate(ts);
		}
}
void Scene::render()
{
	Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
	Light::RenderCommand::clear();
    Light::Renderer::beginScene(m_camera, m_lightPos);
		
	m_skybox.render();
	for(int i=0;i<m_gameObjects.size();i++){
		m_gameObjects[i].render();
	}
	
	m_floor.render();

	Light::Renderer::endScene();
}
void Scene::onEvent(Light::Event& e) 
{
    m_camera.onEvent(e);
	for(int i=0;i<m_gameObjects.size();i++){
		m_gameObjects[i].onEvent(e);
	}
    
	
}

void Scene:: onWindowResize(Light::WindowResizeEvent& e){

		auto[width, height] = e.getSize();
		m_camera.setViewportSize(width, height);

		
}
float Scene::getlightPosx(){
    return {m_lightPos.x};
}
void Scene::addgameObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
   Cube obj=Cube(position,rotation,scale);
   m_gameObjects.push_back(obj);

}