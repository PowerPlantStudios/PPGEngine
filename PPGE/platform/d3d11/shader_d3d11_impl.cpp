#include "shader_d3d11_impl.h"

#include "platform/d3d11/device_d3d11_impl.h"
#include <d3dcompiler.h>

namespace PPGE
{
static inline std::string GetShaderProfile(const ShaderDesc &desc)
{
    std::string profile;

    // clang-format off
    switch (desc.shader_type_flags)
    {
    case PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX:        profile = "vs"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL:         profile = "ps"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_GEOMETRY:      profile = "gs"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_HULL:          profile = "hs"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_DOMAIN:        profile = "ds"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_COMPUTE:       profile = "cs"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_AMPLIFICATION: profile = "as"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_MESH:          profile = "ms"; break;
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_GEN:
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_MISS:
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_CLOSEST_HIT:
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_ANY_HIT:
    case PPGE::ShaderTypeFlags::SHADER_TYPE_RAY_INTERSECTION:
    case PPGE::ShaderTypeFlags::SHADER_TYPE_CALLABLE:     profile = "lib"; break;
    default:
        PPGE_ASSERT(false, "Unknown shader type flag.");
        return "";
    }
    // clang-format on

    // TODO: This shader model MAJOR_MINOR will be parametrized later.
    profile.append("_5_0");

    return profile;
}

ShaderD3D11Impl::ShaderD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const ShaderCreateDesc &create_desc)
    : ShaderBaseType(std::move(device_sp), create_desc.desc)
{
    if (create_desc.source_code || create_desc.file_path)
    {
        std::string_view source_code;
        if (create_desc.source_code == nullptr)
        {
            // TODO: Read shader code from file.
        }
        else
        {
            source_code = std::string_view(create_desc.source_code, create_desc.source_code_size);
        }

        DWORD shader_flags = 0;
#ifdef PPGE_DEBUG
        shader_flags |= 1 << 0;
#endif
        std::string shader_profile = GetShaderProfile(m_desc);

        CComPtr<ID3DBlob> compiler_output_ptr;
        HRESULT hr = D3DCompile(source_code.data(), source_code.size(), nullptr /* Source Name */, nullptr /* Macros */,
                                nullptr /* Include Implementation */, create_desc.entry_point_name,
                                shader_profile.c_str(), shader_flags, 0, &m_shader_byte_code_ptr, &compiler_output_ptr);
        if (FAILED(hr))
        {
            PPGE_ERROR("Compilation of {0} shader failed.", ShaderTypeToString(m_desc.shader_type_flags));
            if (compiler_output_ptr->GetBufferSize() > 0)
            {
                std::string_view compiler_msg(static_cast<char *>(compiler_output_ptr->GetBufferPointer()),
                                              compiler_output_ptr->GetBufferSize());
                PPGE_ERROR("D3D Compiler Output: {0}", compiler_msg);
            }
            PPGE_ASSERT(false, "D3D11 Shader creation failed: There is an error in shader compilation.");
        }
    }
    else if (create_desc.byte_code)
    {
        PPGE_ASSERT(create_desc.byte_code_size > 0,
                    "D3D11 Shader creation failed: Byte code size must be greater than 0 when byte code is provided.");
        PPGE_HR(D3DCreateBlob(create_desc.byte_code_size, &m_shader_byte_code_ptr));
        memcpy(m_shader_byte_code_ptr->GetBufferPointer(), create_desc.byte_code, create_desc.byte_code_size);
    }

    PPGE_ASSERT(m_shader_byte_code_ptr->GetBufferSize() > 0,
                "D3D11 Shader creation failed: Shader byte code is empty.");

    auto *d3d11_device = m_device_sp->GetD3D11Device();
    switch (m_desc.shader_type_flags)
    {
    case PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX: {
        CComPtr<ID3D11VertexShader> d3d11_vertex_shader_ptr;
        d3d11_device->CreateVertexShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                         m_shader_byte_code_ptr->GetBufferSize(), nullptr, &d3d11_vertex_shader_ptr);
        d3d11_vertex_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                                reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    case PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL: {
        CComPtr<ID3D11PixelShader> d3d11_pixel_shader_ptr;
        d3d11_device->CreatePixelShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                        m_shader_byte_code_ptr->GetBufferSize(), nullptr, &d3d11_pixel_shader_ptr);
        d3d11_pixel_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                               reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    case PPGE::ShaderTypeFlags::SHADER_TYPE_GEOMETRY: {
        CComPtr<ID3D11GeometryShader> d3d11_geometry_shader_ptr;
        d3d11_device->CreateGeometryShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                           m_shader_byte_code_ptr->GetBufferSize(), nullptr,
                                           &d3d11_geometry_shader_ptr);
        d3d11_geometry_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                                  reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    case PPGE::ShaderTypeFlags::SHADER_TYPE_HULL: {
        CComPtr<ID3D11HullShader> d3d11_hull_shader_ptr;
        d3d11_device->CreateHullShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                       m_shader_byte_code_ptr->GetBufferSize(), nullptr, &d3d11_hull_shader_ptr);
        d3d11_hull_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                              reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    case PPGE::ShaderTypeFlags::SHADER_TYPE_DOMAIN: {
        CComPtr<ID3D11DomainShader> d3d11_domain_shader_ptr;
        d3d11_device->CreateDomainShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                         m_shader_byte_code_ptr->GetBufferSize(), nullptr, &d3d11_domain_shader_ptr);
        d3d11_domain_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                                reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    case PPGE::ShaderTypeFlags::SHADER_TYPE_COMPUTE: {
        CComPtr<ID3D11ComputeShader> d3d11_compute_shader_ptr;
        d3d11_device->CreateComputeShader(m_shader_byte_code_ptr->GetBufferPointer(),
                                          m_shader_byte_code_ptr->GetBufferSize(), nullptr, &d3d11_compute_shader_ptr);
        d3d11_compute_shader_ptr->QueryInterface(__uuidof(ID3D11DeviceChild),
                                                 reinterpret_cast<void **>(&m_d3d11_shader_ptr));
        break;
    }
    default:
        PPGE_ASSERT(false, "D3D11 Shader creation failed: Unsupported shader type for D3D11 API: {0}",
                    ShaderTypeToString(m_desc.shader_type_flags));
        break;
    }

    PPGE_ASSERT(m_d3d11_shader_ptr, "D3D11 Shader creation failed: Cannot create {0}.",
                ShaderTypeToString(m_desc.shader_type_flags));
}

ShaderD3D11Impl::~ShaderD3D11Impl()
{
}
} // namespace PPGE