#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/descriptors.h"
#include "system/logger_system.h"

namespace PPGE
{
class RendererSystemDX11;

class PPGE_API ShaderD3D11
{
  public:
    ShaderD3D11();

    ~ShaderD3D11();

    void Destroy();
    bool Create(const ShaderDesc &desc);
    void Set();

    inline ID3DBlob *const GetShaderBuffer() const
    {
        return m_buffer;
    }

  private:
    RendererSystemDX11 *m_renderer;
    ID3DBlob *m_buffer;
    union {
        ID3D11PixelShader *m_pixel_shader;
        ID3D11VertexShader *m_vertex_shader;
        ID3D11ComputeShader *m_compute_shader;
        ID3D11GeometryShader *m_geometry_shader;
        ID3D11DomainShader *m_domain_shader;
        ID3D11HullShader *m_hull_shader;
        IUnknown *m_shader;
    };
    ShaderDesc::ShaderType m_type;
};

class PPGE_API ProgramD3D11
{
  public:
    ProgramD3D11();

    ~ProgramD3D11();

    void Destroy();
    bool Create(const ProgramDesc &desc);

    inline ShaderHandle GetVSHandle() const
    {
        return m_vs_handle;
    }
    inline ShaderHandle GetPSHandle() const
    {
        return m_ps_handle;
    }

  private:
    ShaderHandle m_vs_handle;
    ShaderHandle m_ps_handle;
};
} // namespace PPGE