#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/swap_chain.h"
#include "system/logger_system.h"

class GLFWwindow;

namespace PPGE
{
class PPGE_API SwapChainGL : public SwapChain
{
  public:
    SwapChainGL(GLFWwindow *window) : m_window_ptr(window)
    {
        PPGE_ASSERT(m_window_ptr, "GLFWwindow is null");
    }

    void Swap() override;

  private:
    GLFWwindow *m_window_ptr;
};
} // namespace PPGE