#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGEBuffer;
class PPGEShaderResourceBinding;
class PPGETextureView;

class Material
{
  public:
    virtual void Build(std::shared_ptr<PPGEShaderResourceBinding> &SRB, std::shared_ptr<PPGEBuffer> &buffer) = 0;
};

class LegacyMaterial : public Material
{
  public:
    struct MaterialDesc
    {
        Math::Color albedo_color = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
        Math::Color emissive_color = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
        float shininess = 0.0f;
        float alpha_cutoff = 1.0f;
    };

    LegacyMaterial() = default;
    ~LegacyMaterial() = default;

    LegacyMaterial(const MaterialDesc &desc);

    void Build(std::shared_ptr<PPGEShaderResourceBinding> &SRB, std::shared_ptr<PPGEBuffer> &buffer) override;

#define SetMap(ID, MAP)                                                                                                \
    inline void Set##ID##Map(std::shared_ptr<PPGETextureView> MAP##_map)                                               \
    {                                                                                                                  \
        m_##MAP##_map = std::move(##MAP##_map);                                                                        \
    }
    SetMap(Albedo, albedo) SetMap(Specular, specular) SetMap(Normal, normal) SetMap(Emission, emission);
#undef SetMap

  private:
    std::shared_ptr<PPGETextureView> m_albedo_map;
    std::shared_ptr<PPGETextureView> m_specular_map;
    std::shared_ptr<PPGETextureView> m_normal_map;
    std::shared_ptr<PPGETextureView> m_emission_map;

    Math::Color m_albedo_color;
    Math::Color m_emissive_color;
    float m_shininess;
    float m_alpha_cutoff;
};

class PBRMaterial : public Material
{
  public:
    struct MaterialDesc
    {
        Math::Color albedo_color = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
        Math::Color emissive_color = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
        float roughness_factor = 1.0f;
        float metalic_factor = 0.0f;
        float alpha_cutoff = 1.0f;
    };

    PBRMaterial() = default;
    ~PBRMaterial() = default;

    PBRMaterial(const MaterialDesc &desc);

    void Build(std::shared_ptr<PPGEShaderResourceBinding> &SRB, std::shared_ptr<PPGEBuffer> &buffer) override;

#define SetMap(ID, MAP)                                                                                                \
    inline void Set##ID##Map(std::shared_ptr<PPGETextureView> MAP##_map)                                               \
    {                                                                                                                  \
        m_##MAP##_map = std::move(##MAP##_map);                                                                        \
    }

    SetMap(Albedo, albedo) SetMap(Roughness, roughness) SetMap(Metallic, metallic) SetMap(Normal, normal);
    SetMap(Occlusion, occlusion) SetMap(Emission, emission) SetMap(Height, height) SetMap(AlphaMask, alpha_mask);
#undef SetMap

  private:
    std::shared_ptr<PPGETextureView> m_albedo_map;
    std::shared_ptr<PPGETextureView> m_roughness_map;
    std::shared_ptr<PPGETextureView> m_metallic_map;
    std::shared_ptr<PPGETextureView> m_normal_map;
    std::shared_ptr<PPGETextureView> m_occlusion_map;
    std::shared_ptr<PPGETextureView> m_emission_map;
    std::shared_ptr<PPGETextureView> m_height_map;
    std::shared_ptr<PPGETextureView> m_alpha_mask_map;

    Math::Color m_albedo_color;
    Math::Color m_emissive_color;
    float m_roughness_factor;
    float m_metalic_factor;
    float m_alpha_cutoff;
};

namespace MaterialHelper
{
std::shared_ptr<PPGETextureView> LoadTexture(const std::filesystem::path &path_to_texture);

std::shared_ptr<PPGETextureView> LoadTexture(uint32_t height, uint32_t width, const void *data, const char *format);

template <typename MaterialType, typename... Args> std::shared_ptr<MaterialType> CreateMaterial(Args &&...args)
{
    return std::make_shared<MaterialType>(std::forward<Args>(args)...);
}
} // namespace MaterialHelper
} // namespace PPGE
