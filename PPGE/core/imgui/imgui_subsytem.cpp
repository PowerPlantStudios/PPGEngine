#include "PPGEpch.h"

#include "imgui_subsytem.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "core/application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PPGE
{
ImGuiSubsystem::ImGuiSubsystem() : Subsystem("ImGuiSubsystem")
{
}

ImGuiSubsystem::~ImGuiSubsystem()
{
}

void ImGuiSubsystem::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application &app = Application::Get();
    GLFWwindow *window = static_cast<GLFWwindow *>(DisplaySystem::GetDisplaySystem().GetNativeDisplayPtr());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiSubsystem::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiSubsystem::OnInputEvent(InputEvent &input_event)
{
    ImGuiIO &io = ImGui::GetIO();
    input_event.SetHandled(input_event.IsInCategory(InputEventCategoryBit::Mouse) & io.WantCaptureMouse);
    input_event.SetHandled(input_event.IsInCategory(InputEventCategoryBit::Keyboard) & io.WantCaptureKeyboard);
}

void ImGuiSubsystem::InitFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiSubsystem::RenderFrame()
{
    ImGuiIO &io = ImGui::GetIO();
    Application &app = Application::Get();
    io.DisplaySize = ImVec2((float)DisplaySystem::GetDisplaySystem().GetWidth(),
                            (float)DisplaySystem::GetDisplaySystem().GetHeight());

    // Rendering
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
} // namespace PPGE