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

VertexBufferHandle RendererSystemGL::CreateVertexBuffer(const VertexBufferDesc &desc)
{
    return VertexBufferHandle();
}

IndexBufferHandle RendererSystemGL::CreateIndexBuffer(const IndexBufferDesc &desc)
{
    return IndexBufferHandle();
}

TextureHandle RendererSystemGL::CreateTexture(const TextureDesc &desc)
{
    return TextureHandle();
}

ShaderHandle RendererSystemGL::CreateShader(const ShaderDesc &desc)
{
    return ShaderHandle();
}

void RendererSystemGL::ReleaseVertexBuffer(VertexBufferHandle &hnd)
{
}

void RendererSystemGL::ReleaseIndexBuffer(IndexBufferHandle &hnd)
{
}

void RendererSystemGL::ReleaseTexture(TextureHandle &hnd)
{
}

void RendererSystemGL::ReleaseShader(ShaderHandle &hnd)
{
}

void RendererSystemGL::LoadVertexBuffer(const VertexBufferHandle &hnd)
{
}

void RendererSystemGL::UnloadVertexBuffer(const VertexBufferHandle &hnd)
{
}

void RendererSystemGL::LoadIndexBuffer(const IndexBufferHandle &hnd)
{
}

void RendererSystemGL::UnloadIndexBuffer(const IndexBufferHandle &hnd)
{
}

void RendererSystemGL::LoadTexture(const TextureHandle &hnd)
{
}

void RendererSystemGL::UnloadTexture(const TextureHandle &hnd)
{
}

void RendererSystemGL::LoadShader(const ShaderHandle &hnd)
{
}

void RendererSystemGL::UnloadShader(const ShaderHandle &hnd)
{
}

void RendererSystemGL::OnResize()
{
}
void RendererSystemGL::Submit()
{
}
} // namespace PPGE