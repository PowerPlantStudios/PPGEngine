#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "renderer/frame.h"
#include "renderer/render_states.h"
#include "renderer/renderer_handles.h"
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

struct PredefinedUniform
{
    enum Handle
    {
        Transform0 = 0,
        Transform1,
        Transform2,
        Transform3,
        Light0,
        Light1,
        Light2,
        Light3,

        // Reserved

        Count = PPGE_RENDERER_PREDEFINED_UNIFORMS
    };

    UniformHandle handle;
    Subresource subresource;
    UniformDesc::Target target;
    uint8_t slot;
};

class PPGE_API RendererSystem : public ISystem<RendererSystemProps>
{
  public:
    virtual void Update() = 0;

    virtual void OnResize() = 0;

    virtual RendererAPI GetRendererAPI() = 0;

    virtual bool ClearColor(float r, float g, float b) = 0;
    virtual bool ClearDepthStencilBuffer(float depth, uint8_t stencil) = 0;

    virtual bool CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle) = 0;
    virtual bool ReleaseVertexBuffer(VertexBufferHandle handle) = 0;

    virtual bool CreateVertexLayout(const VertexLayout &layout, VertexLayoutHandle handle) = 0;
    virtual bool ReleaseVertexLayout(VertexLayoutHandle handle) = 0;

    virtual bool CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle) = 0;
    virtual bool ReleaseIndexBuffer(IndexBufferHandle handle) = 0;

    virtual bool CreateTexture(const TextureDesc &desc, TextureHandle handle) = 0;
    virtual bool ReleaseTexture(TextureHandle handle) = 0;

    virtual bool CreateProgram(const ProgramDesc &desc, ProgramHandle handle) = 0;
    virtual bool ReleaseProgram(ProgramHandle handle) = 0;

    virtual bool CreateShader(const ShaderDesc &desc, ShaderHandle handle) = 0;
    virtual bool ReleaseShader(ShaderHandle handle) = 0;

    virtual bool CreateUniform(const UniformDesc &desc, UniformHandle handle) = 0;
    virtual bool ReleaseUniform(UniformHandle handle) = 0;

    virtual bool SetPredefinedUniform(const PredefinedUniform &uniform) = 0;
    virtual bool SetRenderStates(const RenderStates &states) = 0;

    virtual bool Submit(const Frame &frame) = 0;

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