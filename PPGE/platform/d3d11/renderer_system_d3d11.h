#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/os_helper.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
class ImGuiWidgetD3D11;
class DeviceD3D11Impl;
class DeviceContextD3D11Impl;
class SwapChainD3D11Impl;

class PPGE_API RendererSystemD3D11 final : public RendererSystem
{
  public:
    RendererSystemD3D11()
    {
    }
    ~RendererSystemD3D11()
    {
    }

    void StartUp(const RendererSystemProps &props) override final;
    void Update() override final;
    void ShutDown() override final;

    void OnResize() override final;

    RendererAPIType GetRendererAPI() override final
    {
        return RendererAPIType::D3D11;
    }

    PPGEDevice *GetDevice() override final;

    PPGEDeviceContext *GetImmediateContext() override final;

    PPGESwapChain *GetSwapChain() override final;

  private:
    std::shared_ptr<DeviceD3D11Impl> m_device_sp;
    std::shared_ptr<DeviceContextD3D11Impl> m_device_context_sp;
    std::shared_ptr<SwapChainD3D11Impl> m_swap_chain_sp;

    friend ImGuiWidgetD3D11;
};
} // namespace PPGE
