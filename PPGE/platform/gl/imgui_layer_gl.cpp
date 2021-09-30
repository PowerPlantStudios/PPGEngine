#include "imgui_layer_gl.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>

#include "system/display_system.h"

namespace PPGE
{
void ImGuiLayerGL::OnAttach()
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

    GLFWwindow *window = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayerGL::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PPGE::ImGuiLayerGL::OnImGuiBegin()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayerGL::OnRender()
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)DisplaySystem::Get().GetWidth(), (float)DisplaySystem::Get().GetHeight());

    // Rendering
    bool show_demo_window = true;
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

void ImGuiLayerGL::OnInputEvent(InputEvent &event)
{
    ImGuiIO &io = ImGui::GetIO();
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Mouse) & io.WantCaptureMouse);
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Keyboard) & io.WantCaptureKeyboard);
}

void ImGuiLayerGL::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE
