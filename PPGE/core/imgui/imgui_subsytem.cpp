#include "PPGEpch.h"

#include "imgui_subsytem.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "core/application.h"

namespace PPGE
{
	ImGuiSubsystem::ImGuiSubsystem()
		: Subsystem("ImGuiSubsystem")
	{
	}

	ImGuiSubsystem::~ImGuiSubsystem()
	{
	}

	void ImGuiSubsystem::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		Application& application = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(application.GetDisplay().GetNativeDisplayPtr());
		
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiSubsystem::OnDetach()
	{
	}
	
	void ImGuiSubsystem::OnUpdate(float timestamp)
	{
	}
	
	void ImGuiSubsystem::OnInputEvent(InputEvent&)
	{
	}
}