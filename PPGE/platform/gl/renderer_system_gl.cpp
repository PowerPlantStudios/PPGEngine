#include "platform/gl/renderer_system_gl.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "system/display_system.h"

namespace PPGE
{
void RendererSystemGL::StartUp(const RendererSystemProps &props)
{
    m_window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    PPGE_ASSERT(m_window_ptr, "Handle to the GLFW window is empty.");

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

void RendererSystemGL::Update()
{
    glfwSwapBuffers(m_window_ptr);
}

void RendererSystemGL::ShutDown()
{
}

void RendererSystemGL::OnResize()
{
}

bool RendererSystemGL::CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::SetVertexBuffer(VertexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::ReleaseVertexBuffer(VertexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::SetIndexBuffer(IndexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::ReleaseIndexBuffer(IndexBufferHandle handle)
{
    return false;
}

bool RendererSystemGL::CreateTexture(const TextureDesc &desc, TextureHandle handle)
{
    return false;
}

bool RendererSystemGL::SetTexture(TextureHandle handle, Sampler sampler)
{
    return false;
}

bool RendererSystemGL::ReleaseTexture(TextureHandle handle)
{
    return false;
}

bool RendererSystemGL::CreateUniform(const UniformDesc &desc, UniformHandle handle)
{
    return false;
}

bool RendererSystemGL::SetUniform(UniformHandle handle, void *data)
{
    return false;
}

bool RendererSystemGL::SetRenderStates(const RenderStates &states)
{
    return false;
}

bool RendererSystemGL::Submit(ProgramHandle handle)
{
    return false;
}

} // namespace PPGE