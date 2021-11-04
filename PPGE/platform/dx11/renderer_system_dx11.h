#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "platform/dx11/context_dx11.h"
#include "platform/dx11/device_dx11.h"
#include "platform/dx11/swap_chain_dx11.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class RendererContextDX11;

class PPGE_API RendererSystemDX11 : public RendererSystem
{
  public:
    RendererSystemDX11()
    {
        m_depth_stencil_buffer = NULL;
        m_depth_stencil_view = NULL;
        m_render_target_view = NULL;

        ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
        
    }
    ~RendererSystemDX11()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::DX11;
    }

    void CreateDevice() override;
    void CreateContext() override;
    void CreateSwapChain() override;

    Context &GetContext() override;
    SwapChain &GetSwapChain() override;

    void OnResize() override;

  private:
    bool m_enable_4x_msaa = true;

    std::unique_ptr<DeviceDX11> m_device;
    std::unique_ptr<ContextDX11> m_context;
    std::unique_ptr<SwapChainDX11> m_swap_chain;
    
    ID3D11Texture2D *m_depth_stencil_buffer;
    ID3D11DepthStencilView *m_depth_stencil_view;
    ID3D11RenderTargetView *m_render_target_view;

    D3D11_VIEWPORT m_viewport;
    PPGE::Math::Color green = PPGE::Math::Color(0.0f, 1.0f, 0.0f);
};
} // namespace PPGE