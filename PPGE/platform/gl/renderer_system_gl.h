#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/gl/context_gl.h"
#include "platform/gl/swap_chain_gl.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class PPGE_API RendererSystemGL : public RendererSystem
{
  public:
    RendererSystemGL()
    {
    }
    ~RendererSystemGL()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::OpenGL;
    }

    void CreateDevice() override;
    void CreateContext() override;
    void CreateSwapChain() override;

    Context &GetContext() override;
    SwapChain &GetSwapChain() override;

    void OnResize() override;

  private:
    std::unique_ptr<ContextGL> m_context;
    std::unique_ptr<SwapChainGL> m_swap_chain;
};
} // namespace PPGE