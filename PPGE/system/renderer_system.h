#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "renderer/render_states.h"
#include "renderer/renderer_handles.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "renderer/uniform.h"
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
    virtual void OnResize() = 0;

    virtual RendererAPI GetRendererAPI() = 0;

    virtual VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc) = 0;
    virtual void SetVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void ReleaseVertexBuffer(VertexBufferHandle &handle) = 0;

    virtual IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc) = 0;
    virtual void SetIndexBuffer(IndexBufferHandle handle) = 0;
    virtual void ReleaseIndexBuffer(IndexBufferHandle &handle) = 0;

    virtual TextureHandle CreateTexture(const TextureDesc &desc) = 0;
    virtual void SetTexture(TextureHandle handle, Sampler sampler) = 0;
    virtual void ReleaseTexture(TextureHandle &handle) = 0;

    virtual UniformHandle CreateUniform(const UniformDesc &desc) = 0;
    virtual void SetUniform(UniformHandle handle, void *data) = 0;

    virtual void SetRenderStates(const RenderStates &states) = 0;

    virtual void Submit(ProgramHandle handle) = 0;

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

  protected:
    template <typename RenderSystemImpl> static RenderSystemImpl *GetRendererSystem()
    {
        if (s_instance)
            return static_cast<RenderSystemImpl *>(s_instance);
        return nullptr;
    }

  private:
    static RendererSystem *s_instance;
};
} // namespace PPGE