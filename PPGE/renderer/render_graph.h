#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/render_pass.h"
#include "rhi/device_object.h"
#include "system/logger_system.h"

namespace PPGE
{
class RenderGraph
{
  public:
    RenderGraph() = default;

    template <typename PassType, typename std::enable_if_t<std::is_base_of_v<RenderPass, PassType>> * = nullptr>
    PassType &CreateRenderPass()
    {
        auto pass = std::make_unique<PassType>();

        return PushRenderPass(std::move(pass));
    }

    template <typename PassType, typename std::enable_if_t<std::is_base_of_v<RenderPass, PassType>> * = nullptr>
    PassType &PushRenderPass(std::unique_ptr<PassType> render_pass)
    {
        m_passes.push_back(std::move(render_pass));

        return static_cast<PassType &>(*(m_passes.back()));
    }

    virtual void Compile();

    void Run();

    virtual void Destroy()
    {
        m_resources.clear();
        m_passes.clear();
    }

    template <typename PPGEDeviceObjectType,
              typename std::enable_if_t<std::is_base_of_v<PPGEDeviceObject, PPGEDeviceObjectType>> * = nullptr>
    std::shared_ptr<PPGEDeviceObjectType> GetResource(const std::string &resource_name) const
    {
        auto it = m_resources.find(resource_name);
        if (it == m_resources.end())
            return nullptr;
        return std::static_pointer_cast<PPGEDeviceObjectType>(it->second);
    }

    void PushResource(const std::string &resource_name, std::shared_ptr<PPGEDeviceObject> resource_sp)
    {
        auto it = m_resources.find(resource_name);
        if (it == m_resources.end())
            m_resources.emplace(resource_name, std::move(resource_sp));
    }

  private:
    void ParsePassResources(const std::vector<RenderPassResource> &resources);

  protected:
    std::unordered_map<std::string, std::shared_ptr<PPGEDeviceObject>> m_resources;

  private:
    std::vector<std::unique_ptr<RenderPass>> m_passes;
};

class SceneRenderGraph : public RenderGraph
{
  public:
    template <typename PassType, typename std::enable_if_t<std::is_base_of_v<RenderPass, PassType>> * = nullptr>
    PassType &CreateRenderPass()
    {
        if constexpr (std::is_base_of_v<SceneRenderPass, PassType>)
        {
            auto &pass = RenderGraph::CreateRenderPass<PassType>();
            m_srp_refs.push_back(pass);

            return pass;
        }
        else
        {
            return RenderGraph::CreateRenderPass<PassType>();
        }
    }

    template <typename PassType, typename std::enable_if_t<std::is_base_of_v<RenderPass, PassType>> * = nullptr>
    PassType &PushRenderPass(std::unique_ptr<PassType> render_pass)
    {
        if constexpr (std::is_base_of_v<SceneRenderPass, PassType>)
        {
            auto &pass = RenderGraph::PushRenderPass(std::move(render_pass));
            m_srp_refs.push_back(pass);

            return pass;
        }
        else
        {
            return RenderGraph::PushRenderPass(std::move(render_pass));
        }
    }

    void BindScene(const SceneRenderPassData &data);

    void UnbindScene();

    virtual void Compile() override;

    virtual void Destroy() override
    {
        m_cb_per_frame.reset();
        m_srp_refs.clear();
        RenderGraph::Destroy();
    }

  private:
    std::vector<std::reference_wrapper<SceneRenderPass>> m_srp_refs;

    std::shared_ptr<PPGEBuffer> m_cb_per_frame;
};
} // namespace PPGE
