#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/buffer.h"
#include "rhi/texture.h"
#include "system/logger_system.h"

namespace PPGE
{
class RenderGraph;
class Scene;
class CameraController;

enum class RenderPassResourceType
{
    UNDEFINED = 0,
    TEXTURE,
    BUFFER
};

struct RenderPassResourceDesc
{
    virtual RenderPassResourceType GetType() const = 0;
};

struct RenderPassTextureResourceDesc final : public RenderPassResourceDesc
{
    TextureDesc rhi_desc;

    constexpr RenderPassTextureResourceDesc() = default;

    constexpr RenderPassTextureResourceDesc(TextureDesc &&tex_desc) : RenderPassResourceDesc(), rhi_desc{tex_desc}
    {
    }

    RenderPassResourceType GetType() const override final
    {
        return RenderPassResourceType::TEXTURE;
    }
};

struct RenderPassBufferResourceDesc final : public RenderPassResourceDesc
{
    BufferDesc rhi_desc;

    RenderPassResourceType GetType() const override final
    {
        return RenderPassResourceType::BUFFER;
    }
};

struct RenderPassResource
{
    const char *name;
    const RenderPassResourceDesc &resource_desc;
};

class RenderPass
{
  public:
    virtual void Load(RenderGraph &render_graph) = 0;

    virtual void Unload() = 0;

    virtual void Execute() = 0;

    virtual std::vector<RenderPassResource> GetPassInputs() const
    {
        return {};
    }

    virtual std::vector<RenderPassResource> GetPassOutputs() const
    {
        return {};
    }
};

struct SceneRenderPassData
{
    const Scene &scene;
    const CameraController &active_camera;
};

class SceneRenderPass : public RenderPass
{
  public:
    void BindSceneData(const SceneRenderPassData &data)
    {
        m_data = &data;
    }

    void UnbindSceneData()
    {
        m_data = nullptr;
    }

  protected:
    const SceneRenderPassData &GetSceneDataRef() const
    {
        PPGE_ASSERT(
            m_data,
            "Invalid reference to scene at scene render pass. Possibly, no scene is bound before accessing it.");
        return *m_data;
    }

  private:
    const SceneRenderPassData *m_data = nullptr;
};
} // namespace PPGE
