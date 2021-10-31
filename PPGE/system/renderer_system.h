#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
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