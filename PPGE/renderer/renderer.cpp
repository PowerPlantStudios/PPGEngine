#include "renderer.h"

#include "ecs/scene.h"
#include "renderer/passes/clear_buffer_pass.h"
#include "renderer/passes/forward_render_pass.h"
#include "renderer/passes/present_pass.h"

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
    m_active_srgh.CreateRenderPass<ClearBufferPass>();
    m_active_srgh.CreateRenderPass<ForwardRenderPass>();
    m_active_srgh.Compile();
}

SceneRenderer Renderer::BeginScene(const Scene &scene, const CameraController &active_camera)
{
    return SceneRenderer(m_active_srgh, scene, active_camera);
}
} // namespace PPGE
