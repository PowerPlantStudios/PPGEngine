#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/buffer.h"
#include "renderer/renderer_handles.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
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

    virtual VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc) = 0;
    virtual IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc) = 0;
    virtual TextureHandle CreateTexture(const TextureDesc &desc) = 0;
    virtual ShaderHandle CreateShader(const ShaderDesc &desc) = 0;

    virtual void ReleaseVertexBuffer(VertexBufferHandle &hnd) = 0;
    virtual void ReleaseIndexBuffer(IndexBufferHandle &hnd) = 0;
    virtual void ReleaseTexture(TextureHandle &hnd) = 0;
    virtual void ReleaseShader(ShaderHandle &hnd) = 0;

    virtual void LoadVertexBuffer(const VertexBufferHandle &hnd) = 0;
    virtual void UnloadVertexBuffer(const VertexBufferHandle &hnd) = 0;
    virtual void LoadIndexBuffer(const IndexBufferHandle &hnd) = 0;
    virtual void UnloadIndexBuffer(const IndexBufferHandle &hnd) = 0;
    virtual void LoadTexture(const TextureHandle &hnd) = 0;
    virtual void UnloadTexture(const TextureHandle &hnd) = 0;
    virtual void LoadShader(const ShaderHandle &hnd) = 0;
    virtual void UnloadShader(const ShaderHandle &hnd) = 0;

    virtual void OnResize() = 0;

    virtual void Submit() = 0;

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

    template <typename Renderer> static Renderer *GetRenderer()
    {
        if (s_instance)
            return static_cast<Renderer *>(s_instance);
        return nullptr;
    }

  private:
    static RendererSystem *s_instance;
};
} // namespace PPGE