#include "render_graph.h"

#include "renderer/camera_controller.h"
#include "renderer/renderer_types.h"
#include "rhi/buffer.h"
#include "rhi/texture.h"
#include "system/renderer_system.h"

namespace PPGE
{
void RenderGraph::Compile()
{
    for (auto &pass : m_passes)
    {
        ParsePassResources(pass->GetPassInputs());
        ParsePassResources(pass->GetPassOutputs());
    }

    for (auto &pass : m_passes)
    {
        pass->Load(*this);
    }
}

void RenderGraph::Run()
{
    for (auto &pass : m_passes)
    {
        pass->Execute();
    }
}

void RenderGraph::ParsePassResources(const std::vector<RenderPassResource> &resources)
{
    for (const auto &resource : resources)
    {
        auto it = m_resources.find(resource.name);
        if (it != m_resources.end())
            continue;

        std::shared_ptr<PPGEDeviceObject> device_object_sp;
        switch (resource.resource_desc.GetType())
        {
        case RenderPassResourceType::BUFFER: {
            const auto &resourece_desc = static_cast<const RenderPassBufferResourceDesc &>(resource.resource_desc);

            std::shared_ptr<PPGEBuffer> buffer_sp;
            RendererSystem::Get().GetDevice()->CreateBuffer(resourece_desc.rhi_desc, nullptr, buffer_sp);
            device_object_sp = std::static_pointer_cast<PPGEDeviceObject>(std::move(buffer_sp));
            break;
        }
        case RenderPassResourceType::TEXTURE: {
            const auto &resourece_desc = static_cast<const RenderPassTextureResourceDesc &>(resource.resource_desc);

            TextureCreateDesc cd;
            cd.desc = resourece_desc.rhi_desc;
            if ((cd.desc.resource_dimension == ResourceDimensionType::RESOURCE_DIMENSION_2D ||
                 cd.desc.resource_dimension == ResourceDimensionType::RESOURCE_DIMENSION_2D_ARRAY) &&
                (cd.desc.width == 0 || cd.desc.height == 0))
            {
                cd.desc.width = 1280;
                cd.desc.height = 720;
            }
            std::shared_ptr<PPGETexture> texture_sp;
            RendererSystem::Get().GetDevice()->CreateTexture(cd, texture_sp);
            device_object_sp = std::static_pointer_cast<PPGEDeviceObject>(std::move(texture_sp));
            break;
        }
        case RenderPassResourceType::UNDEFINED: {
            PPGE_ASSERT(false, "Undefined render pass resource type.");
            break;
        }
        default: {
            PPGE_ASSERT(false, "Unknown render pass resource type.");
            break;
        }
        }

        if (device_object_sp)
        {
            m_resources.emplace(resource.name, std::move(device_object_sp));
        }
    }
}

void SceneRenderGraph::BindScene(const SceneRenderPassData &data)
{
    // Set per frame camera data
    CbPerFrame *map_data;
    {
        PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_frame.get(), PPGE::MapType::MAP_WRITE,
                                                               PPGE::MapFlags::MAP_DISCARD,
                                                               reinterpret_cast<void **>(&map_data));
        Math::Vector3 camera_position = data.active_camera.GetPosition();

        *map_data =
            CbPerFrame{.view = data.active_camera.GetView().Transpose(),
                       .view_inverse = data.active_camera.GetView().Invert().Transpose(),
                       .proj = data.active_camera.GetProj().Transpose(),
                       .proj_inverse = data.active_camera.GetProj().Invert().Transpose(),
                       .view_proj = data.active_camera.GetViewProj().Transpose(),
                       .view_proj_inverse = data.active_camera.GetViewProj().Invert().Transpose(),
                       .camera_position = Math::Vector4(camera_position.x, camera_position.y, camera_position.z, 1.0f),
                       .camera_direction = Math::Vector4()};
        PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_frame.get());
    }
    // Bind reference to scene data
    for (auto &scene_pass : m_srp_refs)
    {
        scene_pass.get().BindSceneData(data);
    }
}

void SceneRenderGraph::UnbindScene()
{
    for (auto &scene_pass : m_srp_refs)
    {
        scene_pass.get().UnbindSceneData();
    }
}

void SceneRenderGraph::Compile()
{
    BufferDesc cb_desc;
    cb_desc.byte_width = sizeof(CbPerFrame);
    cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
    cb_desc.usage = UsageType::USAGE_DYNAMIC;
    cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
    RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_frame);
    PPGE_ASSERT(m_cb_per_frame, "Scene Render Graph: Creating per frame data buffer has failed.");

    m_resources.emplace(CbPerFrameResourceName, std::move(std::static_pointer_cast<PPGEDeviceObject>(m_cb_per_frame)));

    RenderGraph::Compile();
}
} // namespace PPGE
