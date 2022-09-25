#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_types.h"

namespace PPGE
{
class CameraController;
class PPGEBuffer;
class Renderer;
class Scene;
class SceneRenderer;

class Renderer
{
  public:
    Renderer();

    Renderer(const Renderer &) = delete;

    Renderer &operator=(const Renderer &) = delete;

    SceneRenderer BeginScene(const Scene &scene, const CameraController &active_camera);

    const SceneRenderGraph &GetActiveSceneRGH() const
    {
        return m_active_scene_rgh;
    }

    void SetRendererOptions(RendererOptions options);

    RendererOptions GetRendererOptions() const
    {
        return m_options;
    }

  private:
    void SubmitRendererOptions();

    void ReloadRenderGraph();

    std::mutex m_mutex;

    SceneRenderGraph m_active_scene_rgh;

    RendererOptions m_options = RendererOptions::NONE;

    Math::Vector2 m_resolution = Math::Vector2(1280.0f, 720.0f);

    float m_shadowmap_resolution = 2048.0f;

    std::shared_ptr<PPGEBuffer> m_cb_renderer_options;
};

class SceneRenderer
{
  public:
    SceneRenderer(std::mutex &renderer_mutex, SceneRenderGraph &scene_render_graph, const Scene &scene,
                  const CameraController &active_camera);

    ~SceneRenderer();

    void Submit();

  private:
    std::mutex &m_renderer_mutex;

    SceneRenderGraph &m_scene_render_graph;

    SceneRenderPassData m_data;
};
} // namespace PPGE
