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

    bool ClearColor(float r, float g, float b) override;
    bool ClearDepthStencilBuffer(float depth, uint8_t stencil) override;

    bool CreateVertexBuffer(const VertexBufferDesc &desc, VertexBufferHandle handle) override;
    bool ReleaseVertexBuffer(VertexBufferHandle handle) override;

    bool CreateVertexLayout(const VertexLayout &layout, VertexLayoutHandle handle) override;
    bool ReleaseVertexLayout(VertexLayoutHandle handle) override;

    bool CreateIndexBuffer(const IndexBufferDesc &desc, IndexBufferHandle handle) override;
    bool ReleaseIndexBuffer(IndexBufferHandle handle) override;

    bool CreateTexture(const TextureDesc &desc, TextureHandle handle) override;
    bool ReleaseTexture(TextureHandle handle) override;

    bool CreateProgram(const ProgramDesc &desc, ProgramHandle handle) override;
    bool CreateShader(const ShaderDesc &desc, ShaderHandle handle) override;
    
    bool CreateUniform(const UniformDesc &desc, UniformHandle handle) override;
    bool UpdateUniform(UniformHandle handle, const SubResource &resource) override;
    bool SetUniform(UniformHandle handle, ShaderDesc::ShaderType target, uint8_t slot) override;
    bool ReleaseUniform(UniformHandle handle) override;

    bool SetRenderStates(const RenderStates &states) override;

    bool Submit(const Frame &frame) override;

  private:
    GLFWwindow *m_window_ptr;
};
} // namespace PPGE