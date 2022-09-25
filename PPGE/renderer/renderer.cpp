#include "renderer.h"

#include "ecs/scene.h"
#include "renderer/passes/clear_buffer_pass.h"
#include "renderer/passes/forward_render_pass.h"
#include "renderer/passes/geometry_pass.h"
#include "renderer/passes/lighting_pass.h"
#include "renderer/passes/present_pass.h"
#include "renderer/passes/shadow_pass.h"
#include "renderer/shader_library.h"
#include "rhi/buffer.h"
#include "system/renderer_system.h"

namespace PPGE
{
SceneRenderer::SceneRenderer(std::mutex &renderer_mutex, SceneRenderGraph &scene_render_graph, const Scene &scene,
                             const CameraController &active_camera)
    : m_renderer_mutex{renderer_mutex}, m_scene_render_graph{scene_render_graph}, m_data{.scene = scene,
                                                                                         .active_camera = active_camera}
{
    m_renderer_mutex.lock();
    m_scene_render_graph.BindScene(m_data);
}

SceneRenderer::~SceneRenderer()
{
    m_scene_render_graph.UnbindScene();
    m_renderer_mutex.unlock();
}

void SceneRenderer::Submit()
{
    m_scene_render_graph.Run();
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

    ReloadRenderGraph();

    ShaderLibrary::Get().Subscribe("Renderer", [this](auto &shader_path) { ReloadRenderGraph(); });
}

SceneRenderer Renderer::BeginScene(const Scene &scene, const CameraController &active_camera)
{
    return SceneRenderer(m_mutex, m_active_scene_rgh, scene, active_camera);
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

void Renderer::ReloadRenderGraph()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    {
        m_active_scene_rgh.Destroy();
        m_active_scene_rgh.PushResource(CbRendererOptionsName, m_cb_renderer_options);
        m_active_scene_rgh.CreateRenderPass<ClearBufferPass>();
        m_active_scene_rgh.CreateRenderPass<ShadowPass>();
        m_active_scene_rgh.CreateRenderPass<GeometryPass>();
        m_active_scene_rgh.CreateRenderPass<LightingPass>();
        m_active_scene_rgh.CreateRenderPass<PresentPass>();
        m_active_scene_rgh.Compile();
    }
}
} // namespace PPGE
