#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/isystem.h"
#include "ui/ui_layer.h"

namespace PPGE
{
struct UISystemProps
{
};

class PPGE_API UISystem : public ISystem<UISystemProps>
{
    using shared_layer = std::shared_ptr<UILayer>;
    using layer_container = std::vector<shared_layer>;

  public:
    UISystem()
    {
    }
    ~UISystem()
    {
    }

    virtual void StartUp(const UISystemProps &props) override;
    virtual void Update() override;
    virtual void ShutDown() override;

    template <typename LayerType, typename = std::enable_if_t<std::is_base_of_v<UILayer, LayerType>>>
    std::weak_ptr<LayerType> PushLayerFront(std::unique_ptr<LayerType> layer)
    {
        std::shared_ptr<LayerType> _layer(std::move(layer));
        m_managed_layers.insert(m_managed_layers.begin(), _layer);
        _layer->OnAttach();
        return _layer;
    }

    template <typename LayerType, typename = std::enable_if_t<std::is_base_of_v<UILayer, LayerType>>>
    std::weak_ptr<LayerType> PushLayerBack(std::unique_ptr<LayerType> layer)
    {
        std::shared_ptr<LayerType> _layer(std::move(layer));
        m_managed_layers.emplace_back(_layer);
        _layer->OnAttach();
        return _layer;
    }

    template <typename LayerType, typename = std::enable_if_t<std::is_base_of_v<UILayer, LayerType>>>
    void RemoveLayer(std::weak_ptr<LayerType> layer)
    {
        shared_layer _layer = layer.lock();
        auto &&it = std::find(m_managed_layers.begin(), m_managed_layers.end(), _layer);
        if (it != m_managed_layers.end())
        {
            (*it)->OnDetach();
            m_managed_layers.erase(it);
        }
    }

    layer_container::iterator begin()
    {
        return m_managed_layers.begin();
    }
    layer_container::iterator end()
    {
        return m_managed_layers.end();
    }
    layer_container::reverse_iterator rbegin()
    {
        return m_managed_layers.rbegin();
    }
    layer_container::reverse_iterator rend()
    {
        return m_managed_layers.rend();
    }
    layer_container::const_iterator begin() const
    {
        return m_managed_layers.begin();
    }
    layer_container::const_iterator end() const
    {
        return m_managed_layers.end();
    }
    layer_container::const_reverse_iterator rbegin() const
    {
        return m_managed_layers.rbegin();
    }
    layer_container::const_reverse_iterator rend() const
    {
        return m_managed_layers.rend();
    }

  private:
    layer_container m_managed_layers;

  public:
    static void Initialize();

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static UISystem &Get()
    {
        return *s_instance;
    }

  private:
    static UISystem *s_instance;
};
} // namespace PPGE