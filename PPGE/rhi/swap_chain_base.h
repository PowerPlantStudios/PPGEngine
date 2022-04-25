#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/swap_chain.h"
#include "system/logger_system.h"

namespace PPGE
{
template <typename RendererTraits, typename SwapChainInterface> class PPGE_API SwapChainBase : public SwapChainInterface
{
  public:
    using DeviceImplType = typename RendererTraits::DeviceImpl;
    using DeviceContextImplType = typename RendererTraits::DeviceContextImpl;

    SwapChainBase(std::weak_ptr<DeviceImplType> device_wp, std::weak_ptr<DeviceContextImplType> immediate_context_wp,
                  const SwapChainDesc &desc)
        : m_device_wp(std::move(device_wp)), m_immediate_context_wp(std::move(immediate_context_wp)), m_desc(desc)

    {
    }

    ~SwapChainBase()
    {
    }

    const SwapChainDesc &GetDesc() const override final
    {
        return m_desc;
    }

    void Resize(uint32_t width, uint32_t height) override final
    {
        if (width != 0 && height != 0 && (m_desc.width != width || m_desc.height != height))
        {
            m_desc.width = width;
            m_desc.height = height;
            UpdateSwapChain(false);
        }
    }

  protected:
    virtual void UpdateSwapChain(bool recreate) = 0;

    std::weak_ptr<DeviceImplType> m_device_wp;
    std::weak_ptr<DeviceContextImplType> m_immediate_context_wp;

    SwapChainDesc m_desc;
};
} // namespace PPGE