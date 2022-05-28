#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"
#include "rhi/device_context.h"

namespace PPGE
{
class LightComponent;
class TransformComponent;
class PPGETextureView;

class Light
{
  public:
    Light(const TransformComponent &transform_component, const LightComponent &light_component)
        : m_transform_comp(transform_component), m_light_comp(light_component)
    {
    }

    size_t GetShadowMapViewCount() const;

    Math::Matrix GetLightView(size_t index) const;

    Math::Matrix GetLightProj() const;

    std::shared_ptr<PPGETextureView> GetShadowMapDSViewAt(size_t index = 0) const;

    Viewport GetShadowMapViewportAt(size_t index = 0) const;

    std::shared_ptr<PPGETextureView> GetShadowMapSRView() const;

  private:
    const TransformComponent &m_transform_comp;
    
    const LightComponent &m_light_comp;
};
} // namespace PPGE