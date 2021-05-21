#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "Objects.hpp"

#include <stdio.h>

#include "btBulletDynamicsCommon.h"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), 
			camera(45.0f, 1.6f/0.9f, 0.001f, 100.0f),
			lightPos(-1,2,1.5),
			floor(glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(2,0.1,2)),
			cube(glm::vec3(-1,2,0), glm::vec3(0,-30.0f,10.0f))

	{
	  // bullet code
	  btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration () ;
	  btCollisionDispatcher * dispatcher = new btCollisionDispatcher ( collisionConfiguration ) ;
	  btBroadphaseInterface * overlappingPairCache = new btDbvtBroadphase () ;
	  btSequentialImpulseConstraintSolver * solver = new btSequentialImpulseConstraintSolver ;
	  btDiscreteDynamicsWorld * dynamicsWorld = new btDiscreteDynamicsWorld ( dispatcher ,
                                                                              overlappingPairCache ,
																			  solver ,
																			  collisionConfiguration ) ;
	 dynamicsWorld -> setGravity ( btVector3 (0 , -10 ,0) ) ;
	 	
	 {
		 btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1), btScalar(.05), btScalar(1)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		
		dynamicsWorld->addRigidBody(body);

	 }
	 {
		 //create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(.5), btScalar(.5), btScalar(0.5)));
		
		collisionShapes.push_back(colShape);

		
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setFromOpenGLMatrix(glm::value_ptr(cube.getTransform()));

		btScalar mass(1.f);

		
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		

		
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
	 }																	  
	
	
	
		// Rest of initialization
		texture.reset(Light::Texture2D::create("../Light/assets/textures/check.png"));

		Light::FramebufferSpec fbspec;
		fbspec.width = 1280;
		fbspec.height = 720;

		framebuffer = Light::Framebuffer::create(fbspec);
	}

	~ExampleLayer()
	{
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	
	delete dynamicsWorld;

	
	delete solver;

	
	delete overlappingPairCache;

	
	delete dispatcher;

	delete collisionConfiguration;

	
	collisionShapes.clear();
	}

	void onUpdate(Light::Timestep ts) override
	{
		 dynamicsWorld->stepSimulation(ts.getSeconds(), 10);
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[1];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
		else
			{
				trans = obj->getWorldTransform();
			}
		glm::mat4 Tcube=glm::mat4(1.0f);
		trans.getOpenGLMatrix(glm::value_ptr(Tcube));
		cube.setTransform(Tcube);
		


		if(resizeViewport)
		{
			camera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);
			framebuffer->resize(viewportPanelSize.x, viewportPanelSize.y);
			resizeViewport = false;
		}

		framecount++;
		time += ts.getMilliSeconds();
		if(time >= 500.0f)
		{
			lastTime = time;
			lastFramecount = framecount;
			time = 0.0f;
			framecount = 0;
		}

		camera.onUpdate(ts);
		cube.onUpdate(ts);

		framebuffer->bind();

		Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
		Light::RenderCommand::clear();

		Light::Renderer::beginScene(camera, lightPos);
		
		skybox.render();
		cube.render();
		floor.render();

		Light::Renderer::endScene();

		framebuffer->unbind();
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto[width, height] = e.getSize();
		camera.setViewportSize(width, height);

		return false;
	}


	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResize));

		camera.onEvent(e);

		cube.onEvent(e);
	}

	void onImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		Light::Application::get().getImguiLayer()->blockFocusEvents(!ImGui::IsWindowFocused());
		Light::Application::get().getImguiLayer()->blockHoverEvents(!ImGui::IsWindowHovered());

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if(viewportPanelSize != *(glm::vec2*)&panelSize)
		{
			resizeViewport = true;
			viewportPanelSize = glm::vec2(panelSize.x, panelSize.y);
		}
		ImGui::Image((void*)framebuffer->getColorAttachmentRendererId(), panelSize, ImVec2(0, 1), ImVec2(1,0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Scene Settings");
		ImGui::DragFloat3("Light Position", &(lightPos.x), 0.01f);
		ImGui::End();

		ImGui::Begin("Camera Controls");
		ImGui::Text("Left Alt + LMB to Orbit");
		ImGui::Text("Left Alt + MMB to Pan");
		ImGui::Text("Left Alt + RMB to Zoom");
		ImGui::Text("Scroll to Zoom");
		ImGui::End();

		ImGui::Begin("Performance Statistics");
		ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d", 
					lastTime/lastFramecount,
					lastTime*0.001f/lastFramecount,
					int(lastFramecount*1000/lastTime));
		ImGui::End();

	}

private:
	Light::EditorCamera camera;
	Cube cube;
	Cube floor;
	Skybox skybox;
	glm::vec3 lightPos;
	btDefaultCollisionConfiguration * collisionConfiguration  ;
	btCollisionDispatcher * dispatcher  ;
	btBroadphaseInterface * overlappingPairCache  ;
	btSequentialImpulseConstraintSolver * solver  ;
	 btDiscreteDynamicsWorld * dynamicsWorld  ;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	std::shared_ptr<Light::Framebuffer> framebuffer;
	std::shared_ptr<Light::Texture2D> texture;

	glm::vec2 viewportPanelSize;
	bool resizeViewport = false;

	float time = 0.0f;
	int framecount = 0;
	float lastTime = 0.0f;
	int lastFramecount = 0;

};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}