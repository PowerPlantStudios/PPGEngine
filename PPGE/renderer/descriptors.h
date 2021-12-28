#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/renderer_handles.h"
#include "renderer/vertex_layout.h"

namespace PPGE
{
struct Subresource
{
    void *m_pData = nullptr;
    size_t m_size = 0;
};

struct BufferDesc
{
    enum class UsageType
    {
        Default = 0,
        Immutable,
        Dynamic,
        Staging
    };

    enum class CPUFlag
    {
        None = 0,
        Read,
        Write
    };

    Subresource m_resource;
    UsageType m_usage = UsageType::Default;
    CPUFlag m_cpu_flags = CPUFlag::None;
};

struct IndexBufferDesc : public BufferDesc
{
    unsigned int m_index_cout;
};

struct VertexBufferDesc : public BufferDesc
{
    VertexLayout m_layout;
};

enum class ShaderResourceTarget
{
    VS,
    HS,
    DS,
    GS,
    PS
};

struct TextureDesc
{
    enum class TextureFormat
    {
        RGBA8 = 0,
        RGBA8I,
        RGBA8U,
        RGBA8S,
        RGBA16,
        RGBA16I,
        RGBA16U,
        RGBA16F,
        RGBA16S,
        RGBA32,
        RGBA32I,
        RGBA32U,
        RGBA32F,
        RGBA32S,

        D16,
        D24,
        D24S8,
        D32,
        D16F,
        D24F,
        D32F
    };

    TextureFormat m_format;
    void *m_data;
    size_t m_width;
};

struct Texture2DDesc : public TextureDesc
{
    size_t m_height;
};

struct Texture3DDesc : public Texture2DDesc
{
    size_t m_depth;
};

struct TextureResurceDesc
{
    enum class FileFormat
    {
        DDS = 0,
        TGA,
        HDR,
        BMP,
        JPEG,
        PNG,
        TIFF
    };

    FileFormat m_format;
    std::filesystem::path m_path;
};

struct SamplerDesc
{

};

struct ShaderDesc
{
    enum class ShaderType
    {
        Unknown,
        VS,
        HS,
        DS,
        GS,
        PS
    };

    ShaderType m_type;
    const char *m_data;
    size_t m_size;
};

struct ProgramDesc
{
    ShaderHandle m_vertex_shader_handle;
    ShaderHandle m_pixel_shader_handle;
};

struct UniformDesc
{
    enum class UniformType
    {
        Scalar,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4
    };

    const char *m_name;
    UniformType m_type;
    uint16_t m_num;
};
} // namespace PPGE