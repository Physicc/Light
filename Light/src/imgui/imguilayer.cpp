#include "imgui/imguilayer.hpp"
#include "core/application.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Light
{
	ImguiLayer::~ImguiLayer() {}

	void ImguiLayer::onAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImguiLayer::onDetach()
	{

	}

	void ImguiLayer::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(onMouseButtonPressedEvent));
		d.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(onMouseButtonReleasedEvent));
		d.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMovedEvent));
		d.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(onMouseScrolledEvent));
		d.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(onKeyPressedEvent));
		d.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(onKeyReleasedEvent));
		d.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(onKeyTypedEvent));
		d.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResizeEvent));
	}

	void ImguiLayer::onUpdate()
	{

		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		float time = (float)glfwGetTime();
		float delta = (time > 0.0f) ? (time - this->time) : (1.0f/60.0f);
		this->time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool ImguiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();

		io.MouseDown[e.getButton()] = true;

		return false;
	}
	
	bool ImguiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();

		io.MouseDown[e.getButton()] = false;

		return false;
	}
	
	bool ImguiLayer::onMouseMovedEvent(MouseMovedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)std::get<0>(e.getPos()), (float)std::get<1>(e.getPos()));
		std::cerr << io.MousePos.x << "," << io.MousePos.y << std::endl;

		return false;
	}
	
	bool ImguiLayer::onMouseScrolledEvent(MouseScrolledEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)std::get<0>(e.getOffset());
		io.MouseWheel += (float)std::get<1>(e.getOffset());

		return false;
	}
	
	bool ImguiLayer::onKeyPressedEvent(KeyPressedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeycode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}
	
	bool ImguiLayer::onKeyReleasedEvent(KeyReleasedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeycode()] = false;

		return false;
	}

	bool ImguiLayer::onKeyTypedEvent(KeyTypedEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.getKeycode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)e.getKeycode());

		return false;
	}
	
	bool ImguiLayer::onWindowResizeEvent(WindowResizeEvent& e) 
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(std::get<0>(e.getSize()),std::get<1>(e.getSize()));
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, std::get<0>(e.getSize()),std::get<1>(e.getSize()));
		return false;
	}
}
