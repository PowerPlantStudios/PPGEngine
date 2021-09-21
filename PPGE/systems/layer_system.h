#pragma once

#include "core/defines.h"
#include "layers/layer.h"
#include "systems/isystem.h"

namespace PPGE
{
struct LayerSystemProps
{
};

class PPGE_API LayerSystem : public ISystem<LayerSystemProps>
{
  public:
    LayerSystem()
    {
    }
    ~LayerSystem()
    {
    }

    virtual void StartUp(const LayerSystemProps &props) override;
    virtual void Update() override;
    virtual void ShutDown() override;

    void PushFrontQueue(Layer *subsystem);
    void PushBackQueue(Layer *subsystem);
    void PopFrontQueue(Layer *subsystem);
    void PopBackQueue(Layer *subsystem);

    std::vector<Layer *>::iterator begin()
    {
        return m_ManagedSubsytems.begin();
    }
    std::vector<Layer *>::iterator end()
    {
        return m_ManagedSubsytems.end();
    }
    std::vector<Layer *>::reverse_iterator rbegin()
    {
        return m_ManagedSubsytems.rbegin();
    }
    std::vector<Layer *>::reverse_iterator rend()
    {
        return m_ManagedSubsytems.rend();
    }

    std::vector<Layer *>::const_iterator begin() const
    {
        return m_ManagedSubsytems.begin();
    }
    std::vector<Layer *>::const_iterator end() const
    {
        return m_ManagedSubsytems.end();
    }
    std::vector<Layer *>::const_reverse_iterator rbegin() const
    {
        return m_ManagedSubsytems.rbegin();
    }
    std::vector<Layer *>::const_reverse_iterator rend() const
    {
        return m_ManagedSubsytems.rend();
    }

    inline static LayerSystem &GetLayerSystem()
    {
        return s_instance;
    }

  private:
    std::vector<Layer *> m_ManagedSubsytems;
    unsigned int m_InsertIndex = 0;

    static LayerSystem &s_instance;
};

} // namespace PPGE