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
    RendererSystemGL()
    {
    }
    ~RendererSystemGL()
    {
    }

    void StartUp(const RendererSystemProps &props) override;
    void Update() override;
    void ShutDown() override;

    RendererAPI GetRendererAPI() override
    {
        return RendererAPI::OpenGL;
    }

    VertexBufferHandle CreateVertexBuffer(const VertexBufferDesc &desc) override;
    IndexBufferHandle CreateIndexBuffer(const IndexBufferDesc &desc) override;
    TextureHandle CreateTexture(const TextureDesc &desc) override;
    ShaderHandle CreateShader(const ShaderDesc &desc) override;

    void ReleaseVertexBuffer(VertexBufferHandle &hnd) override;
    void ReleaseIndexBuffer(IndexBufferHandle &hnd) override;
    void ReleaseTexture(TextureHandle &hnd) override;
    void ReleaseShader(ShaderHandle &hnd) override;

    void LoadVertexBuffer(const VertexBufferHandle &hnd) override;
    void UnloadVertexBuffer(const VertexBufferHandle &hnd) override;
    void LoadIndexBuffer(const IndexBufferHandle &hnd) override;
    void UnloadIndexBuffer(const IndexBufferHandle &hnd) override;
    void LoadTexture(const TextureHandle &hnd) override;
    void UnloadTexture(const TextureHandle &hnd) override;
    void LoadShader(const ShaderHandle &hnd) override;
    void UnloadShader(const ShaderHandle &hnd) override;

    void OnResize() override;

    void Submit() override;

  private:
    GLFWwindow *m_window_ptr;
};
} // namespace PPGE