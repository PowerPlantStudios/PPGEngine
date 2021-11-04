#include "platform/gl/context_gl.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "system/display_system.h"

namespace PPGE
{
ContextGL::ContextGL(GLFWwindow *window) : m_window_ptr(window)
{
    PPGE_ASSERT(m_window_ptr, "GLFWwindow is null");

    glfwMakeContextCurrent(m_window_ptr);
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
} // namespace PPGE