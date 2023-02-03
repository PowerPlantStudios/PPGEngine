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

#if !defined(PPGE_PLATFORM_APPLE)
    Math::Vector3 eye = Math::Vector3::Zero;
    Math::Vector3 target = Math::Vector3::Forward;
    Math::Vector3 up = Math::Vector3::Up;
#else
    Math::Vector3 eye = Math::Vector3_Zero;
    Math::Vector3 target =Math::Vector3_Forward;
    Math::Vector3 up = Math::Vector3_Up;
#endif
    LightComponent::LightType type;
    switch (m_light_comp.GetLightType())
    {
    case PPGE::LightComponent::LightType::DIRECTIONAL: {
#if !defined(PPGE_PLATFORM_APPLE)
        eye = Math::Matrix::CreateFromQuaternion(m_transform_comp.rotation).Forward();
        target = Math::Vector3_Zero;
#else
        eye = Math::Forward(Math::CreateFromQuaternion(m_transform_comp.rotation));
        target = Math::Vector3_Zero;
#endif
        break;
    }
    case PPGE::LightComponent::LightType::POINT: {
        eye = m_transform_comp.position;
        switch (index)
        {
        case 0: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Left;
#else
            target = eye + Math::Vector3_Left;
#endif
            break;
        }
        case 1: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Right;
#else
            target = eye + Math::Vector3_Right;
#endif
            break;
        }
        case 2: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Up;
            up = Math::Vector3::Backward;
#else
            target = eye + Math::Vector3_Up;
            up = eye + Math::Vector3_Backward;
#endif
            break;
        }
        case 3: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Down;
            up = Math::Vector3::Forward;
#else
            target = eye + Math::Vector3_Down;
            up = eye + Math::Vector3_Forward;
#endif
            break;
        }
        case 4: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Backward;
#else
            target = eye + Math::Vector3_Backward;
#endif
            break;
        }
        case 5: {
#if !defined(PPGE_PLATFORM_APPLE)
            target = eye + Math::Vector3::Forward;
#else
            target = eye + Math::Vector3_Forward;
#endif
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
#if !defined(PPGE_PLATFORM_APPLE)
        target = eye + Math::Matrix::CreateFromQuaternion(m_transform_comp.rotation).Backward();
#else
        target = eye + Math::Backward(Math::CreateFromQuaternion(m_transform_comp.rotation));
#endif
        break;
    }
    default: {
        PPGE_ASSERT(false, "Unknown light type");
        break;
    }
    }
#if !defined(PPGE_PLATFORM_APPLE)
    return Math::Matrix::CreateLookAt(eye, target, up);
#else
    return Math::CreateLookAt(eye, target, up);
#endif
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
