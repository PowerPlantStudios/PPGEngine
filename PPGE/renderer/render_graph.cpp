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

void RenderGraph::ParsePassResources(const std::vector<RenderPassResource> &bla)
{
    for (const auto &pass_input : bla)
    {
        auto it = m_resources.find(pass_input.name);
        if (it != m_resources.end())
            continue;

        std::shared_ptr<PPGEDeviceObject> device_object_sp;
        switch (pass_input.resource_desc.GetType())
        {
        case RenderPassResourceType::BUFFER: {
            const auto &resourece_desc = static_cast<const RenderPassBufferResourceDesc &>(pass_input.resource_desc);

            std::shared_ptr<PPGEBuffer> buffer_sp;
            RendererSystem::Get().GetDevice()->CreateBuffer(resourece_desc.rhi_desc, nullptr, buffer_sp);
            device_object_sp = std::static_pointer_cast<PPGEDeviceObject>(std::move(buffer_sp));
            break;
        }
        case RenderPassResourceType::TEXTURE: {
            const auto &resourece_desc = static_cast<const RenderPassTextureResourceDesc &>(pass_input.resource_desc);

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
            m_resources.emplace(pass_input.name, std::move(device_object_sp));
        }
    }
}

void SceneRenderGraph::BindScene(const SceneRenderPassData &data)
{
    // Set per frame camera data
    CbCameraData *map_data;
    {
        PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_camera_buffer.get(), PPGE::MapType::MAP_WRITE,
                                                               PPGE::MapFlags::MAP_DISCARD,
                                                               reinterpret_cast<void **>(&map_data));
        *map_data = CbCameraData{.view = data.active_camera.GetView().Transpose(),
                                 .proj = data.active_camera.GetProj().Transpose(),
                                 .viewProj = data.active_camera.GetViewProj().Transpose(),
                                 .cameraPosition = data.active_camera.GetPosition(),
                                 .padding = 0.0f};
        PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_camera_buffer.get());
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
    cb_desc.byte_width = sizeof(CbCameraData);
    cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
    cb_desc.usage = UsageType::USAGE_DYNAMIC;
    cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
    RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_camera_buffer);
    PPGE_ASSERT(m_cb_camera_buffer, "Scene Render Graph: Creating per frame data buffer has failed.");

    m_resources.emplace(CbCameraDataResourceName,
                        std::move(std::static_pointer_cast<PPGEDeviceObject>(m_cb_camera_buffer)));

    RenderGraph::Compile();
}
} // namespace PPGE
