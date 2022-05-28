#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_graph.h"
#include "renderer/renderer_types.h"

namespace PPGE
{
class Scene;
class Renderer;
class CameraController;
class PPGEBuffer;

class SceneRenderer
{
  public:
    SceneRenderer(SceneRenderGraph &scene_rgh, const Scene &scene, const CameraController &active_camera);

    ~SceneRenderer();

    void Submit();

  private:
    SceneRenderGraph &m_scene_rgh;
    SceneRenderPassData m_data;
};

class Renderer
{
  public:
    Renderer();

    Renderer(const Renderer &) = delete;

    Renderer &operator=(const Renderer &) = delete;

    SceneRenderer BeginScene(const Scene &scene, const CameraController &active_camera);

    const SceneRenderGraph &GetActiveSRGH() const
    {
        return m_active_srgh;
    }

    void SetRendererOptions(RendererOptions options);

    RendererOptions GetRendererOptions() const
    {
        return m_options;
    }

  private:
    void SubmitRendererOptions();

    SceneRenderGraph m_active_srgh;

    RendererOptions m_options = RendererOptions::NONE;

    Math::Vector2 m_resolution = Math::Vector2(1280.0f, 720.0f);

    float m_shadowmap_resolution = 2048.0f;

    std::shared_ptr<PPGEBuffer> m_cb_renderer_options;
};
} // namespace PPGE
