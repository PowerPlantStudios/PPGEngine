#include "texture_d3d11_impl.h"

#include "platform/d3d11/device_context_d3d11_impl.h"
#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/renderer_types_d3d11_conversion.h"
#include "platform/d3d11/texture_view_d3d11_impl.h"

namespace PPGE
{

class TextureDescFromD3D11Texture
{
  public:
    TextureDesc operator()(CComPtr<ID3D11Texture1D> d3d11_texture1D_ptr)
    {
        D3D11_TEXTURE1D_DESC desc;
        d3d11_texture1D_ptr->GetDesc(&desc);

        TextureDesc texture_desc;
        texture_desc.resource_dimension = desc.ArraySize > 1 ? ResourceDimensionType::RESOURCE_DIMENSION_1D_ARRAY
                                                             : ResourceDimensionType::RESOURCE_DIMENSION_1D;
        texture_desc.width = static_cast<uint32_t>(desc.Width);
        texture_desc.height = 1;
        texture_desc.array_size = static_cast<uint32_t>(desc.ArraySize);
        texture_desc.format_type = D3D11TextureFormatToPPGETextureFormat(desc.Format);
        texture_desc.mip_levels = static_cast<uint32_t>(desc.MipLevels);
        texture_desc.sample_count = 1;
        texture_desc.usage = D3D11UsageTypeToPPGEUsageType(desc.Usage);
        texture_desc.bind_flags = D3D11BindFlagsToPPGEBindFlags(desc.BindFlags);
        texture_desc.cpu_access_flags = D3D11CPUAccessFlagsToPPGECPUAccessFlags(desc.CPUAccessFlags);
        texture_desc.misc_flags = 0;

        return texture_desc;
    }

    TextureDesc operator()(CComPtr<ID3D11Texture2D> d3d11_texture2D_ptr)
    {
        D3D11_TEXTURE2D_DESC desc;
        d3d11_texture2D_ptr->GetDesc(&desc);

        TextureDesc texture_desc;
        texture_desc.resource_dimension = desc.ArraySize > 1 ? ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY
                                                             : ResourceDimensionType::RESOURCE_DIMENSION_2D;
        texture_desc.width = static_cast<uint32_t>(desc.Width);
        texture_desc.height = static_cast<uint32_t>(desc.Height);
        texture_desc.array_size = static_cast<uint32_t>(desc.ArraySize);
        texture_desc.format_type = D3D11TextureFormatToPPGETextureFormat(desc.Format);
        texture_desc.mip_levels = static_cast<uint32_t>(desc.MipLevels);
        texture_desc.sample_count = static_cast<uint32_t>(desc.SampleDesc.Count);
        texture_desc.usage = D3D11UsageTypeToPPGEUsageType(desc.Usage);
        texture_desc.bind_flags = D3D11BindFlagsToPPGEBindFlags(desc.BindFlags);
        texture_desc.cpu_access_flags = D3D11CPUAccessFlagsToPPGECPUAccessFlags(desc.CPUAccessFlags);
        texture_desc.misc_flags = 0;

        return texture_desc;
    }

    TextureDesc operator()(CComPtr<ID3D11Texture3D> d3d11_texture3D_ptr)
    {
        D3D11_TEXTURE3D_DESC desc;
        d3d11_texture3D_ptr->GetDesc(&desc);

        TextureDesc texture_desc;
        texture_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_3D;
        texture_desc.width = static_cast<uint32_t>(desc.Width);
        texture_desc.height = static_cast<uint32_t>(desc.Height);
        texture_desc.depth = static_cast<uint32_t>(desc.Depth);
        texture_desc.format_type = D3D11TextureFormatToPPGETextureFormat(desc.Format);
        texture_desc.mip_levels = static_cast<uint32_t>(desc.MipLevels);
        texture_desc.sample_count = 1;
        texture_desc.usage = D3D11UsageTypeToPPGEUsageType(desc.Usage);
        texture_desc.bind_flags = D3D11BindFlagsToPPGEBindFlags(desc.BindFlags);
        texture_desc.cpu_access_flags = D3D11CPUAccessFlagsToPPGECPUAccessFlags(desc.CPUAccessFlags);
        texture_desc.misc_flags = 0;

        return texture_desc;
    }
};

TextureD3D11Impl::TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, const TextureCreateDesc &create_desc)
    : TextureBaseType(std::move(device_sp), create_desc.desc)
{
    if (create_desc.resource_path)
    {
        ID3D11Device *d3d11_device = m_device_sp->GetD3D11Device();
        ID3D11DeviceContext *d3d11_device_context = nullptr;
        {
            auto device_context_sp = m_device_sp->GetImmediateContext().lock();
            PPGE_ASSERT(device_context_sp, "Creating texture has failed: Cannot get immediate context reference.");
            d3d11_device_context = device_context_sp->GetD3D11DeviceContext();
        }
        D3D11_USAGE usage = PPGEUsageTypeToD3D11UsageType(m_desc.usage);
        UINT bind_flags = PPGEBindFlagsToD3D11BindFlags(m_desc.bind_flags);
        UINT cpu_access_flags = PPGECPUAccessFlagsToD3D11CPUAccessFlags(m_desc.cpu_access_flags);
        UINT misc_flags = 0;

        CComPtr<ID3D11ShaderResourceView> default_srv;
        switch (create_desc.file_format)
        {
        case PPGE::TextureFileFormat::DDS:
            DirectX::CreateDDSTextureFromFileEx(
                d3d11_device, d3d11_device_context, std::filesystem::path(create_desc.resource_path).c_str(), 0, usage,
                bind_flags, cpu_access_flags, misc_flags, false, &m_d3d11_texture_ptr, &default_srv);
            break;
        case PPGE::TextureFileFormat::JPEG:
        case PPGE::TextureFileFormat::PNG:
        case PPGE::TextureFileFormat::TIFF:
            DirectX::CreateWICTextureFromFileEx(
                d3d11_device, d3d11_device_context, std::filesystem::path(create_desc.resource_path).c_str(), 0, usage,
                bind_flags, cpu_access_flags, misc_flags, DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
                &m_d3d11_texture_ptr, &default_srv);
            break;
        case PPGE::TextureFileFormat::UNKNOWN:
            PPGE_ASSERT(false, "Creating texture has failed: Texture file format is not defined.");
            break;
        default:
            PPGE_ASSERT(false, "Creating texture has failed: Texture file format is not unknown.");
            break;
        }
        PPGE_HR(default_srv.QueryInterface<ID3D11View>(&m_default_view));
    }
    else
    {
        PPGE_ASSERT((m_desc.usage != UsageType::USAGE_IMMUTABLE) || (create_desc.subresource != nullptr),
                    "Immutable resource cannot be created without initial data.");

        switch (m_desc.resource_dimension)
        {
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D:
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D_ARRAY:
            CreateTexture1D(create_desc);
            break;
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D:
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY:
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE:
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE_ARRAY:
            CreateTexture2D(create_desc);
            break;
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_3D:
            CreateTexture3D(create_desc);
            break;
        case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED:
            PPGE_ASSERT(false, "Texture creation has failed: Resource dimension is undefined.");
            break;
        default:
            PPGE_ASSERT(false, "Texture creation has failed: Unknown resource dimension is given.");
            break;
        }
    }
}

TextureD3D11Impl::TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture1D *texture)
    : TextureBaseType(std::move(device_sp), TextureDescFromD3D11Texture{}(texture))
{
    m_d3d11_texture_ptr = texture;
}

TextureD3D11Impl::TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture2D *texture)
    : TextureBaseType(std::move(device_sp), TextureDescFromD3D11Texture{}(texture))
{
    m_d3d11_texture_ptr = texture;
}

TextureD3D11Impl::TextureD3D11Impl(std::shared_ptr<DeviceD3D11Impl> device_sp, ID3D11Texture3D *texture)
    : TextureBaseType(std::move(device_sp), TextureDescFromD3D11Texture{}(texture))
{
    m_d3d11_texture_ptr = texture;
}

TextureD3D11Impl::~TextureD3D11Impl()
{
}

std::shared_ptr<PPGETextureView> TextureD3D11Impl::CreateView(const TextureViewDesc &desc)
{
    ValidateResourceView(desc.texture_view_type);

    CComPtr<ID3D11View> d3d11_view_ptr;
#define CREATE_RESOURCE_VIEW(VIEW_TYPE, VIEW_NAME)                                                                     \
    {                                                                                                                  \
        CComPtr<ID3D11##VIEW_TYPE##> d3d11_##VIEW_NAME##_ptr;                                                          \
        Create##VIEW_TYPE##(desc, &d3d11_##VIEW_NAME##_ptr);                                                           \
        PPGE_HR(d3d11_##VIEW_NAME##_ptr.QueryInterface<ID3D11View>(&d3d11_view_ptr));                                  \
    }
    switch (desc.texture_view_type)
    {
    case PPGE::ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE:
        CREATE_RESOURCE_VIEW(ShaderResourceView, srv);
        break;
    case PPGE::ResourceViewType::RESOURCE_VIEW_RENDER_TARGET:
        CREATE_RESOURCE_VIEW(RenderTargetView, rtv);
        break;
    case PPGE::ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL:
        CREATE_RESOURCE_VIEW(DepthStencilView, dsv);
        break;
    case PPGE::ResourceViewType::RESOURCE_VIEW_UNDEFINED:
        PPGE_ASSERT(false, "Texture view creation has failed: Resource view type is undefined.");
        break;
    default:
        PPGE_ASSERT(false, "Texture view creation has failed: Unknown texture view type.");
        break;
    }

    return std::make_shared<TextureViewD3D11Impl>(m_device_sp->shared_from_this(), this->shared_from_this(),
                                                  d3d11_view_ptr, desc);
}

std::shared_ptr<PPGETextureView> TextureD3D11Impl::GetDefaultView()
{
    ID3D11View *d3d11_view_ptr = nullptr;
    TextureViewDesc desc;
    /* TODO: Fill desc */

    return std::make_shared<TextureViewD3D11Impl>(m_device_sp->shared_from_this(), this->shared_from_this(),
                                                  m_default_view, desc);
}

void TextureD3D11Impl::CreateTexture1D(const TextureCreateDesc &create_desc)
{
    D3D11_TEXTURE1D_DESC desc;
    desc.Width = m_desc.width;
    desc.MipLevels = m_desc.mip_levels;
    desc.ArraySize = m_desc.array_size;
    desc.Format = PPGETextureFormatToD3D11TextureFormat(m_desc.format_type);
    desc.Usage = PPGEUsageTypeToD3D11UsageType(m_desc.usage);
    desc.BindFlags = PPGEBindFlagsToD3D11BindFlags(m_desc.bind_flags);
    desc.CPUAccessFlags = PPGECPUAccessFlagsToD3D11CPUAccessFlags(m_desc.cpu_access_flags);
    desc.MiscFlags = 0;

    std::vector<D3D11_SUBRESOURCE_DATA> init_data = CreateInitData(create_desc);

    auto d3d11_device = m_device_sp->GetD3D11Device();
    ID3D11Texture1D *d3d11_texture1D_ptr;
    PPGE_HR(d3d11_device->CreateTexture1D(&desc, init_data.empty() ? nullptr : init_data.data(), &d3d11_texture1D_ptr));
    m_d3d11_texture_ptr = d3d11_texture1D_ptr;
}

void TextureD3D11Impl::CreateTexture2D(const TextureCreateDesc &create_desc)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = m_desc.width;
    desc.Height = m_desc.height;
    desc.MipLevels = m_desc.mip_levels;
    desc.ArraySize = m_desc.array_size;
    desc.Format = PPGETextureFormatToD3D11TextureFormat(m_desc.format_type);
    desc.SampleDesc.Count = m_desc.sample_count;
    desc.SampleDesc.Quality = 0;
    desc.Usage = PPGEUsageTypeToD3D11UsageType(m_desc.usage);
    desc.BindFlags = PPGEBindFlagsToD3D11BindFlags(m_desc.bind_flags);
    desc.CPUAccessFlags = PPGECPUAccessFlagsToD3D11CPUAccessFlags(m_desc.cpu_access_flags);
    desc.MiscFlags = (m_desc.resource_dimension == ResourceDimensionType::RESOURCE_DIMENSION_CUBE ||
                      m_desc.resource_dimension == ResourceDimensionType::RESOURCE_DIMENSION_CUBE_ARRAY)
                         ? D3D11_RESOURCE_MISC_TEXTURECUBE
                         : 0;

    std::vector<D3D11_SUBRESOURCE_DATA> init_data = CreateInitData(create_desc);

    auto d3d11_device = m_device_sp->GetD3D11Device();
    ID3D11Texture2D *d3d11_texture2D_ptr;
    PPGE_HR(d3d11_device->CreateTexture2D(&desc, init_data.empty() ? nullptr : init_data.data(), &d3d11_texture2D_ptr));
    m_d3d11_texture_ptr = d3d11_texture2D_ptr;
}

void TextureD3D11Impl::CreateTexture3D(const TextureCreateDesc &create_desc)
{
    D3D11_TEXTURE3D_DESC desc;
    desc.Width = m_desc.width;
    desc.Height = m_desc.height;
    desc.Depth = m_desc.depth;
    desc.MipLevels = m_desc.mip_levels;
    desc.Format = PPGETextureFormatToD3D11TextureFormat(m_desc.format_type);
    desc.Usage = PPGEUsageTypeToD3D11UsageType(m_desc.usage);
    desc.BindFlags = PPGEBindFlagsToD3D11BindFlags(m_desc.bind_flags);
    desc.CPUAccessFlags = PPGECPUAccessFlagsToD3D11CPUAccessFlags(m_desc.cpu_access_flags);
    desc.MiscFlags = 0;

    std::vector<D3D11_SUBRESOURCE_DATA> init_data = CreateInitData(create_desc);

    auto d3d11_device = m_device_sp->GetD3D11Device();
    ID3D11Texture3D *d3d11_texture3D_ptr;
    PPGE_HR(d3d11_device->CreateTexture3D(&desc, init_data.empty() ? nullptr : init_data.data(), &d3d11_texture3D_ptr));
    m_d3d11_texture_ptr = d3d11_texture3D_ptr;
}

std::vector<D3D11_SUBRESOURCE_DATA> TextureD3D11Impl::CreateInitData(const TextureCreateDesc &create_desc)
{
    const auto &desc = create_desc.desc;
    PPGE_ASSERT((create_desc.subresource == nullptr) ||
                    (create_desc.subresource_num == desc.array_size * desc.mip_levels),
                "Resource initialization data size doesn't match with resource description.");
    std::vector<D3D11_SUBRESOURCE_DATA> init_data;
    if (!create_desc.subresource)
        return init_data;
    init_data.resize(create_desc.subresource_num);
    for (uint32_t i = 0; i < create_desc.subresource_num; i++)
    {
        init_data[i].pSysMem = create_desc.subresource[i].data_ptr;
        init_data[i].SysMemPitch = create_desc.subresource[i].pitch;
        init_data[i].SysMemSlicePitch = create_desc.subresource[i].slice_pitch;
    }
    return init_data;
}

void TextureD3D11Impl::CreateShaderResourceView(const TextureViewDesc &view_desc,
                                                ID3D11ShaderResourceView **d3d11_srv_pp)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
    desc.Format = PPGETextureFormatToD3D11TextureFormat(view_desc.format);
    switch (view_desc.resource_dimension)
    {
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D:
        desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE1D;
        desc.Texture1D.MipLevels = view_desc.mip_levels_num;
        desc.Texture1D.MostDetailedMip = view_desc.most_detailed_mip;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D_ARRAY:
        desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY;
        desc.Texture1DArray.MipLevels = view_desc.mip_levels_num;
        desc.Texture1DArray.MostDetailedMip = view_desc.most_detailed_mip;
        desc.Texture1DArray.ArraySize = view_desc.array_slices_num;
        desc.Texture1DArray.FirstArraySlice = view_desc.first_array_slice;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DMS;
            desc.Texture2DMS.UnusedField_NothingToDefine = 0;
        }
        else
        {
            desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipLevels = view_desc.mip_levels_num;
            desc.Texture2D.MostDetailedMip = view_desc.most_detailed_mip;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            desc.Texture2DMSArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DMSArray.FirstArraySlice = view_desc.first_array_slice;
        }
        else
        {
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
            desc.Texture2DArray.MipLevels = view_desc.mip_levels_num;
            desc.Texture2DArray.MostDetailedMip = view_desc.most_detailed_mip;
            desc.Texture2DArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DArray.FirstArraySlice = view_desc.first_array_slice;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_3D:
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
        desc.Texture3D.MipLevels = view_desc.mip_levels_num;
        desc.Texture3D.MostDetailedMip = view_desc.most_detailed_mip;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE:
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        desc.TextureCube.MipLevels = view_desc.mip_levels_num;
        desc.TextureCube.MostDetailedMip = view_desc.most_detailed_mip;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE_ARRAY:
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
        desc.TextureCubeArray.MipLevels = view_desc.mip_levels_num;
        desc.TextureCubeArray.MostDetailedMip = view_desc.most_detailed_mip;
        desc.TextureCubeArray.NumCubes = view_desc.array_slices_num;
        desc.TextureCubeArray.First2DArrayFace = view_desc.first_array_slice;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED:
        PPGE_ASSERT(false, "Creating shader resource view has failed: Resource dimension is undefined.");
        break;
    default:
        PPGE_ASSERT(false, "Creating shader resource view has failed: Resource dimension is unknown.");
        break;
    }

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateShaderResourceView(m_d3d11_texture_ptr, &desc, d3d11_srv_pp));
}

void TextureD3D11Impl::CreateRenderTargetView(const TextureViewDesc &view_desc, ID3D11RenderTargetView **d3d11_rtv_pp)
{
    D3D11_RENDER_TARGET_VIEW_DESC desc{};
    desc.Format = PPGETextureFormatToD3D11TextureFormat(view_desc.format);
    switch (view_desc.resource_dimension)
    {
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D:
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
        desc.Texture1D.MipSlice = view_desc.most_detailed_mip;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D_ARRAY:
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
        desc.Texture1DArray.MipSlice = view_desc.most_detailed_mip;
        desc.Texture1DArray.ArraySize = view_desc.array_slices_num;
        desc.Texture1DArray.FirstArraySlice = view_desc.first_array_slice;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
            desc.Texture2DMS.UnusedField_NothingToDefine = 0;
        }
        else
        {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = view_desc.most_detailed_mip;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
            desc.Texture2DMSArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DMSArray.FirstArraySlice = view_desc.first_array_slice;
        }
        else
        {
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            desc.Texture2DArray.MipSlice = view_desc.most_detailed_mip;
            desc.Texture2DArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DArray.FirstArraySlice = view_desc.first_array_slice;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_3D:
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
        desc.Texture3D.MipSlice = view_desc.most_detailed_mip;
        desc.Texture3D.WSize = view_desc.depth_slices_num;
        desc.Texture3D.FirstWSlice = view_desc.first_depth_slice;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE:
        PPGE_ASSERT(false, "Creating render target view has failed: Cannot create RTV with resource dimension cube.");
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE_ARRAY:
        PPGE_ASSERT(false,
                    "Creating render target view has failed: Cannot create RTV with resource dimension cube array.");
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED:
        PPGE_ASSERT(false, "Creating render target view has failed: Resource dimension is undefined.");
        break;
    default:
        PPGE_ASSERT(false, "Creating render target view has failed: Resource dimension is unknown.");
        break;
    }

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateRenderTargetView(m_d3d11_texture_ptr, &desc, d3d11_rtv_pp));
}

void TextureD3D11Impl::CreateDepthStencilView(const TextureViewDesc &view_desc, ID3D11DepthStencilView **d3d11_dsv_pp)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
    desc.Format = PPGETextureFormatToD3D11TextureFormat(view_desc.format);
    switch (view_desc.resource_dimension)
    {
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D:
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
        desc.Texture1D.MipSlice = view_desc.most_detailed_mip;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_1D_ARRAY:
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
        desc.Texture1DArray.MipSlice = view_desc.most_detailed_mip;
        desc.Texture1DArray.ArraySize = view_desc.array_slices_num;
        desc.Texture1DArray.FirstArraySlice = view_desc.first_array_slice;
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
            desc.Texture2DMS.UnusedField_NothingToDefine = 0;
        }
        else
        {
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = view_desc.most_detailed_mip;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY:
        if (m_desc.sample_count > 1)
        {
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
            desc.Texture2DMSArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DMSArray.FirstArraySlice = view_desc.first_array_slice;
        }
        else
        {
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
            desc.Texture2DArray.MipSlice = view_desc.most_detailed_mip;
            desc.Texture2DArray.ArraySize = view_desc.array_slices_num;
            desc.Texture2DArray.FirstArraySlice = view_desc.first_array_slice;
        }
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_3D:
        PPGE_ASSERT(false, "Creating depth stencil view has failed: Cannot create DSV with 3D resource dimention.");
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE:
        PPGE_ASSERT(false, "Creating depth stencil view has failed: Cannot create DSV with resource dimension cube.");
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_CUBE_ARRAY:
        PPGE_ASSERT(false,
                    "Creating depth stencil view has failed: Cannot create DSV with resource dimension cube array.");
        break;
    case PPGE::ResourceDimensionType::RESOURCE_DIMENSION_UNDEFINED:
        PPGE_ASSERT(false, "Creating depth stencil view has failed: Resource dimension is undefined.");
        break;
    default:
        PPGE_ASSERT(false, "Creating depth stencil view has failed: Resource dimension is unknown.");
        break;
    }

    auto d3d11_device = m_device_sp->GetD3D11Device();
    PPGE_HR(d3d11_device->CreateDepthStencilView(m_d3d11_texture_ptr, &desc, d3d11_dsv_pp));
}
} // namespace PPGE
