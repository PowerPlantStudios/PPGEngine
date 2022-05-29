#include "light_component.h"

#include "rhi/texture.h"
#include "rhi/texture_view.h"
#include "system/renderer_system.h"

namespace PPGE
{
void LightComponent::CreateShadowMapBuffers()
{
    std::for_each(shadow_map_dsv_array.begin(), shadow_map_dsv_array.end(),
                  [](ShadowMapDSV &shadow_map_dsv) { shadow_map_dsv.dsv.reset(); });
    shadow_map_dsv_array.clear();
    shadow_map_srv.reset();

    TextureCreateDesc cd;
    switch (type)
    {
    case PPGE::LightComponent::LightType::DIRECTIONAL:
        cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY;
        cd.desc.array_size = 1;
        break;
    case PPGE::LightComponent::LightType::POINT:
        cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_CUBE;
        cd.desc.array_size = 6;
        break;
    case PPGE::LightComponent::LightType::SPOT:
        cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        break;
    default:
        PPGE_ASSERT(false, "Unknown light type");
        break;
    }
    cd.desc.width = 2048;
    cd.desc.height = 2048;
    cd.desc.format_type = TextureFormatType::TEXTURE_FORMAT_R32_TYPELESS;
    cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE | BindFlags::BIND_DEPTH_STENCIL;

    std::shared_ptr<PPGETexture> texture_sp;
    RendererSystem::Get().GetDevice()->CreateTexture(cd, texture_sp);

    Viewport shadowmap_viewport;
    shadowmap_viewport.top_left_x = 0;
    shadowmap_viewport.top_left_y = 0;
    shadowmap_viewport.height = 2048;
    shadowmap_viewport.width = 2048;
    shadowmap_viewport.min_depth = 0.0f;
    shadowmap_viewport.max_depth = 1.0f;

    switch (type)
    {
    case PPGE::LightComponent::LightType::DIRECTIONAL: {
        TextureViewDesc dsv_desc;
        dsv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL;
        dsv_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY;
        dsv_desc.format = TextureFormatType::TEXTURE_FORMAT_D32_FLOAT;
        for (unsigned i = 0; i < 1; i++)
        {
            dsv_desc.first_array_slice = i;
            dsv_desc.array_slices_num = 1;
            shadow_map_dsv_array.push_back({std::move(texture_sp->CreateView(dsv_desc)), shadowmap_viewport});
        }
        TextureViewDesc srv_desc;
        srv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        srv_desc.resource_dimension = cd.desc.resource_dimension;
        srv_desc.format = TextureFormatType::TEXTURE_FORMAT_R32_FLOAT;
        srv_desc.first_array_slice = 0;
        srv_desc.array_slices_num = 1;
        shadow_map_srv = texture_sp->CreateView(srv_desc);
        break;
    }
    case PPGE::LightComponent::LightType::POINT: {
        TextureViewDesc dsv_desc;
        dsv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL;
        dsv_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY;
        dsv_desc.format = TextureFormatType::TEXTURE_FORMAT_D32_FLOAT;
        for (unsigned i = 0; i < 6; i++)
        {
            dsv_desc.first_array_slice = i;
            dsv_desc.array_slices_num = 1;
            shadow_map_dsv_array.push_back({std::move(texture_sp->CreateView(dsv_desc)), shadowmap_viewport});
        }
        TextureViewDesc srv_desc;
        srv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        srv_desc.resource_dimension = cd.desc.resource_dimension;
        srv_desc.format = TextureFormatType::TEXTURE_FORMAT_R32_FLOAT;
        shadow_map_srv = texture_sp->CreateView(srv_desc);
        break;
    }
    case PPGE::LightComponent::LightType::SPOT: {
        TextureViewDesc dsv_desc;
        dsv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_DEPTH_STENCIL;
        dsv_desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        dsv_desc.format = TextureFormatType::TEXTURE_FORMAT_D32_FLOAT;
        shadow_map_dsv_array.push_back({std::move(texture_sp->CreateView(dsv_desc)), shadowmap_viewport});
        TextureViewDesc srv_desc;
        srv_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        srv_desc.resource_dimension = cd.desc.resource_dimension;
        srv_desc.format = TextureFormatType::TEXTURE_FORMAT_R32_FLOAT;
        shadow_map_srv = texture_sp->CreateView(srv_desc);
        break;
    }
    default:
        PPGE_ASSERT(false, "Unknown light type");
        break;
    }
}
} // namespace PPGE