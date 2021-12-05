#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

class GLFWwindow;

namespace PPGE
{
class PPGE_API RendererSystemGL : public RendererSystem
{
  public:
    RendererSystemGL() : m_window_ptr(nullptr)
    {
    }
    ~RendererSystemGL()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    void OnResize() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::OpenGL;
    }

    bool CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle) override;
    bool SetVertexBuffer(VertexBufferHandle handle) override;
    bool ReleaseVertexBuffer(VertexBufferHandle handle) override;

    bool CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle) override;
    bool SetIndexBuffer(IndexBufferHandle handle) override;
    bool ReleaseIndexBuffer(IndexBufferHandle handle) override;

    bool CreateTexture(const TextureDesc &desc, TextureHandle handle) override;
    bool SetTexture(TextureHandle handle, Sampler sampler) override;
    bool ReleaseTexture(TextureHandle handle) override;

    bool CreateUniform(const UniformDesc &desc, UniformHandle handle) override;
    bool SetUniform(UniformHandle handle, void *data) override;

    bool SetRenderStates(const RenderStates &states) override;

    bool Submit(ProgramHandle handle) override;

  private:
    GLFWwindow *m_window_ptr;
};
} // namespace PPGE