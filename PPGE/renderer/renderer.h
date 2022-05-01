#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_graph.h"

namespace PPGE
{
class Scene;
class Renderer;
class CameraController;

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

    SceneRenderer BeginScene(const Scene &scene, const CameraController& active_camera);

    const SceneRenderGraph &GetActiveSRGH() const
    {
        return m_active_srgh;
    }

  private:
    SceneRenderGraph m_active_srgh;
};
} // namespace PPGE
