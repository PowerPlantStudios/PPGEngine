#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/context.h"
#include "renderer/swap_chain.h"
#include "system/isystem.h"
#include "system/logger_system.h"

namespace PPGE
{
enum class RendererAPI
{
    None = 0,
    OpenGL,
    Vulkan,
    DX11,
    DX12,
    Metal
};

struct RendererSystemProps
{
};

class PPGE_API RendererSystem : public ISystem<RendererSystemProps>
{
  public:
    virtual RendererAPI GetRendererAPI() = 0;

    virtual void CreateDevice() = 0;
    virtual void CreateContext() = 0;
    virtual void CreateSwapChain() = 0;
    
    virtual Context &GetContext() = 0;
    virtual SwapChain &GetSwapChain() = 0;

    virtual void OnResize() = 0;

  public:
    static void Initialize(RendererAPI api);

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