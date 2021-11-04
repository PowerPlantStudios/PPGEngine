#include "platform/gl/renderer_system_gl.h"

#include <GLFW/glfw3.h>

#include "system/display_system.h"

namespace PPGE
{
void RendererSystemGL::StartUp(const RendererSystemProps &props)
{
    CreateDevice();
    CreateContext();
    CreateSwapChain();
}

void RendererSystemGL::Update()
{
    m_swap_chain->Swap();
}

void RendererSystemGL::ShutDown()
{
    m_swap_chain.reset();
    m_context.reset();
}

void RendererSystemGL::CreateDevice()
{
}

void RendererSystemGL::CreateContext()
{
    m_context = std::make_unique<ContextGL>(static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr()));
}

void RendererSystemGL::CreateSwapChain()
{
    m_swap_chain = std::make_unique<SwapChainGL>(static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr()));
}

Context &RendererSystemGL::GetContext()
{
    return *(m_context.get());
}

SwapChain &RendererSystemGL::GetSwapChain()
{
    return *(m_swap_chain.get());
}

void RendererSystemGL::OnResize()
{
}
} // namespace PPGE