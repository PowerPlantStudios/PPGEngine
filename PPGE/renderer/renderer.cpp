#include "renderer.h"

#include "ecs/scene.h"
#include "renderer/passes/clear_buffer_pass.h"
#include "renderer/passes/forward_render_pass.h"
#include "renderer/passes/present_pass.h"
#include "renderer/passes/shadow_pass.h"
#include "rhi/buffer.h"
#include "system/renderer_system.h"

namespace PPGE
{
SceneRenderer::SceneRenderer(SceneRenderGraph &scene_rgh, const Scene &scene, const CameraController &active_camera)
    : m_scene_rgh{scene_rgh}, m_data{.scene = scene, .active_camera = active_camera}
{
    m_scene_rgh.BindScene(m_data);
}

SceneRenderer::~SceneRenderer()
{
    m_scene_rgh.UnbindScene();
}

void SceneRenderer::Submit()
{
    m_scene_rgh.Run();
}

Renderer::Renderer()
{
    BufferDesc cb_desc;
    cb_desc.byte_width = sizeof(CbRenderer);
    cb_desc.bind_flags = BindFlags::BIND_CONSTANT_BUFFER;
    cb_desc.usage = UsageType::USAGE_DYNAMIC;
    cb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_WRITE;
    RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_renderer_options);
    PPGE_ASSERT(m_cb_renderer_options, "Renderer: Creating renderer options buffer has failed.");

    SubmitRendererOptions();

    m_active_srgh.PushResource(CbRendererOptionsName, m_cb_renderer_options);

    m_active_srgh.CreateRenderPass<ClearBufferPass>();
    m_active_srgh.CreateRenderPass<ShadowPass>();
    m_active_srgh.CreateRenderPass<ForwardRenderPass>();
    m_active_srgh.CreateRenderPass<PresentPass>();

    m_active_srgh.Compile();
}

SceneRenderer Renderer::BeginScene(const Scene &scene, const CameraController &active_camera)
{
    return SceneRenderer(m_active_srgh, scene, active_camera);
}

void Renderer::SetRendererOptions(RendererOptions options)
{
    m_options = options;

    SubmitRendererOptions();
}

void Renderer::SubmitRendererOptions()
{
    CbRenderer *map_data;
    {
        PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_renderer_options.get(), PPGE::MapType::MAP_WRITE,
                                                               PPGE::MapFlags::MAP_DISCARD,
                                                               reinterpret_cast<void **>(&map_data));
        map_data->renderer_options = m_options;
        map_data->render_resolution = m_resolution;
        map_data->shadowmap_resolution = m_shadowmap_resolution;

        PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_renderer_options.get());
    }
}
} // namespace PPGE
