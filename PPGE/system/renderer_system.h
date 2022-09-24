#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/os_helper.h"
#include "rhi/device.h"
#include "rhi/device_context.h"
#include "rhi/swap_chain.h"
#include "system/isystem.h"
#include "system/logger_system.h"

namespace PPGE
{
enum class MSAAQuality
{
    MSAA_OFF = 0,
    MSAA_2X,
    MSAA_4X,
    MSAA_8X,
    MSAA_16X
};

struct RendererSystemProps
{
    MSAAQuality msaa_quality = MSAAQuality::MSAA_4X;
};

class PPGE_API RendererSystem : public ISystem<RendererSystemProps>
{
  public:
    virtual void Update() = 0;

    virtual void OnResize() = 0;

    virtual RendererAPIType GetRendererAPI() = 0;

    virtual PPGEDevice *GetDevice() = 0;

    virtual PPGEDeviceContext *GetImmediateContext() = 0;

    virtual PPGESwapChain *GetSwapChain() = 0;

  public:
    static void Initialize(RendererAPIType api);

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static RendererSystem &Get()
    {
        return *s_instance;
    }

  private:
    static RendererSystem *s_instance;
};
} // namespace PPGE