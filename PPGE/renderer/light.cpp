#include "light.h"

#include "ecs/components/light_component.h"
#include "ecs/components/transform_components.h"
#include "rhi/texture_view.h"

namespace PPGE
{
size_t Light::GetShadowMapViewCount() const
{
    return m_light_comp.GetShadowMapViewCount();
}

Math::Matrix Light::GetLightView(size_t index) const
{
    PPGE_ASSERT(index < GetShadowMapViewCount(), "Index cannot be larger than shado map view count.");

    Math::Vector3 eye = Math::Vector3::Zero;
    Math::Vector3 target = Math::Vector3::Forward;
    Math::Vector3 up = Math::Vector3::Up;

    LightComponent::LightType type;
    switch (m_light_comp.GetLightType())
    {
    case PPGE::LightComponent::LightType::DIRECTIONAL: {
        eye = Math::Matrix::CreateFromQuaternion(m_transform_comp.rotation).Backward();
        target = Math::Vector3::Zero;
        break;
    }
    case PPGE::LightComponent::LightType::POINT: {
        eye = m_transform_comp.position;
        switch (index)
        {
        case 0: {
            target = eye + Math::Vector3::Right;
            break;
        }
        case 1: {
            target = eye + Math::Vector3::Left;
            break;
        }
        case 2: {
            target = eye + Math::Vector3::Up;
            up = Math::Vector3::Backward;
            break;
        }
        case 3: {
            target = eye + Math::Vector3::Down;
            up = Math::Vector3::Forward;
            break;
        }
        case 4: {
            target = eye + Math::Vector3::Forward;
            break;
        }
        case 5: {
            target = eye + Math::Vector3::Backward;
            break;
        }
        default:
            PPGE_ASSERT(false, "Index for point light can only be integer in range [0,5]");
            break;
        }
        break;
    }
    case PPGE::LightComponent::LightType::SPOT: {
        eye = m_transform_comp.position;
        target = eye + Math::Matrix::CreateFromQuaternion(m_transform_comp.rotation).Forward();
        break;
    }
    default: {
        PPGE_ASSERT(false, "Unknown light type");
        break;
    }
    }

    return Math::Matrix::CreateLookAt(eye, target, up);
}

Math::Matrix Light::GetLightProj() const
{
    return m_light_comp.GetProj();
}

std::shared_ptr<PPGETextureView> Light::GetShadowMapDSViewAt(size_t index) const
{
    return m_light_comp.GetShadowMapDSViewAt(index).dsv;
}

Viewport Light::GetShadowMapViewportAt(size_t index) const
{
    return m_light_comp.GetShadowMapDSViewAt(index).viewport;
}

std::shared_ptr<PPGETextureView> Light::GetShadowMapSRView() const
{
    return m_light_comp.GetShadowMapSRView();
}
} // namespace PPGE