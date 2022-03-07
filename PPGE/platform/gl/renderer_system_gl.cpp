#include "platform/gl/renderer_system_gl.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "system/display_system.h"

namespace PPGE
{
void RendererSystemGL::StartUp(const RendererSystemProps &props)
{
    auto window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    PPGE_ASSERT(window_ptr, "Handle to the GLFW window is empty.");

    glfwMakeContextCurrent(window_ptr);
    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PPGE_ASSERT(success, "Failed to initialize GLAD!");
    if (DisplaySystem::Get().IsVsyncEnabled())
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    PPGE_INFO("OpenGL Context Initialized.");
    PPGE_INFO("Using device: Vendor - {0}, Renderer - {1}, Version - {2}", glGetString(GL_VENDOR),
              glGetString(GL_RENDERER), glGetString(GL_VERSION));
    PPGE_ASSERT(GLVersion.major > 4 || (GLVersion.major && GLVersion.minor >= 5),
                "OpenGL version 4.5 and above is required to run PPGE with OpenGL renderer.");
}

void RendererSystemGL::Update()
{
    auto window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    glfwSwapBuffers(window_ptr);
}

void RendererSystemGL::ShutDown()
{
}

void RendererSystemGL::OnResize()
{
}

PPGEDevice *RendererSystemGL::GetDevice()
{
    return nullptr;
}

PPGEDeviceContext *RendererSystemGL::GetImmediateContext()
{
    return nullptr;
}
} // namespace PPGE