#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/swap_chain.h"
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

  protected:
    std::weak_ptr<DeviceImplType> m_device_wp;
    std::weak_ptr<DeviceContextImplType> m_immediate_context_wp;

    SwapChainDesc m_desc;
};
} // namespace PPGE