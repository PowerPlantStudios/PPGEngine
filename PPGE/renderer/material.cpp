#include "material.h"

#include "renderer/renderer_types.h"
#include "system/renderer_system.h"

namespace PPGE
{
static TextureFileFormat GetTextureFileFormat(const char *format)
{
    if (strcmp(format, "dds") == 0)
    {
        return TextureFileFormat::DDS;
    }
    else if (strcmp(format, "jpeg") == 0 || strcmp(format, "jpg") == 0)
    {
        return TextureFileFormat::JPEG;
    }
    else if (strcmp(format, "png") == 0)
    {
        return TextureFileFormat::PNG;
    }
    else if (strcmp(format, "tiff") == 0)
    {
        return TextureFileFormat::TIFF;
    }

    return TextureFileFormat::UNKNOWN;
}

LegacyMaterial::LegacyMaterial(const MaterialDesc &desc)
    : m_albedo_color(desc.albedo_color), m_specular_color(desc.specular_color), m_emissive_color(desc.emissive_color),
      m_shininess(desc.shininess), m_alpha_cutoff(desc.alpha_cutoff)
{
}

void LegacyMaterial::Build(std::shared_ptr<PPGEShaderResourceBinding> &SRB, std::shared_ptr<PPGEBuffer> &buffer)
{
    CbMaterial *material;
    RendererSystem::Get().GetImmediateContext()->Map(buffer.get(), MapType::MAP_WRITE, MapFlags::MAP_DISCARD,
                                                     reinterpret_cast<void **>(&material));
    if (material)
    {
        material->material_options = MaterialOptions::NONE;

        material->albedo_color = m_albedo_color;
        material->specular_color = m_specular_color;
        material->emissive_color = m_emissive_color;

        material->shininess = m_shininess;
        material->alpha_cutoff = m_alpha_cutoff;

#define BindMap(MapName, MapFlag)                                                                                      \
    if (m_##MapName##_map)                                                                                             \
    {                                                                                                                  \
        SRB->GetVariableByName("g_material_" #MapName, ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_##MapName##_map);    \
        material->material_options |= MaterialOptions::##MapFlag##_BOUND;                                              \
    }

        BindMap(albedo, ALBEDO_MAP);
        BindMap(specular, SPECULAR_MAP);
        BindMap(normal, NORMAL_MAP);
        BindMap(emission, EMISSION_MAP);
#undef BindMap

        RendererSystem::Get().GetImmediateContext()->Unmap(buffer.get());
    }
}

PBRMaterial::PBRMaterial(const MaterialDesc &desc)
    : m_albedo_color(desc.albedo_color), m_emissive_color(desc.emissive_color),
      m_roughness_factor(desc.roughness_factor), m_metalic_factor(desc.metalic_factor),
      m_alpha_cutoff(desc.alpha_cutoff)
{
}

void PBRMaterial::Build(std::shared_ptr<PPGEShaderResourceBinding> &SRB, std::shared_ptr<PPGEBuffer> &buffer)
{
    CbMaterial *material;
    RendererSystem::Get().GetImmediateContext()->Map(buffer.get(), MapType::MAP_WRITE, MapFlags::MAP_DISCARD,
                                                     reinterpret_cast<void **>(&material));
    if (material)
    {
        material->material_options = MaterialOptions::NONE;

        material->albedo_color = m_albedo_color;
        material->emissive_color = m_emissive_color;

        material->roughness_factor = m_roughness_factor;
        material->metalic_factor = m_metalic_factor;
        material->alpha_cutoff = m_alpha_cutoff;

#define BindMap(MapName, MapFlag)                                                                                      \
    if (m_##MapName##_map)                                                                                             \
    {                                                                                                                  \
        SRB->GetVariableByName("g_material_" #MapName, ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_##MapName##_map);    \
        material->material_options |= MaterialOptions::##MapFlag##_BOUND;                                              \
    }

        BindMap(albedo, ALBEDO_MAP);
        BindMap(roughness, ROUGHNESS_MAP);
        BindMap(metallic, METALLIC_MAP);
        BindMap(normal, NORMAL_MAP);
        BindMap(occlusion, OCCLUSION_MAP);
        BindMap(emission, EMISSION_MAP);
        BindMap(height, HEIGHT_MAP);
        BindMap(alpha_mask, ALPHA_MASK_MAP);
#undef BindMap

        RendererSystem::Get().GetImmediateContext()->Unmap(buffer.get());
    }
}

std::shared_ptr<PPGETextureView> MaterialHelper::LoadTexture(const std::filesystem::path &path_to_texture)
{
    std::shared_ptr<PPGETexture> texture;
    {
        std::string format = path_to_texture.extension().string().substr(1);
        std::string resource = path_to_texture.string();
        TextureCreateDesc tex_cd;
        tex_cd.resource_path = resource.c_str();
        tex_cd.file_format = GetTextureFileFormat(format.c_str());
        tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;
        RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, texture);
    }

    if (texture)
        return texture->GetDefaultView();
    return nullptr;
}

std::shared_ptr<PPGETextureView> MaterialHelper::LoadTexture(uint32_t height, uint32_t width, const void *data,
                                                             const char *format)
{
    std::shared_ptr<PPGETexture> texture;
    TextureCreateDesc tex_cd;

    // Load uncompressed texture
    if (height != 0)
    {
        tex_cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
        tex_cd.desc.width = width;
        tex_cd.desc.height = height;
        tex_cd.desc.format_type = TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM;
        tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;

        TextureData texture_data;
        texture_data.data_ptr = data;
        texture_data.pitch = width * 4;
        texture_data.slice_pitch = height * width * 4;
        tex_cd.subresource = &texture_data;
        tex_cd.subresource_num = 1;

        RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, texture);
        TextureViewDesc view_desc;
        view_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        view_desc.format = tex_cd.desc.format_type;
        view_desc.resource_dimension = tex_cd.desc.resource_dimension;

        return texture ? texture->CreateView(view_desc) : nullptr;
    }

    // Load compressed texture
    tex_cd.is_compressed = true;
    tex_cd.file_format = GetTextureFileFormat(format);
    TextureData texture_data;
    texture_data.data_ptr = data;
    texture_data.data_size = width;
    tex_cd.subresource = &texture_data;
    tex_cd.subresource_num = 1;
    tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;
    RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, texture);

    if (texture)
        return texture->GetDefaultView();
    return nullptr;
}
} // namespace PPGE
