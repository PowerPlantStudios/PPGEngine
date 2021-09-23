#pragma once

#include "core/defines.h"
#include "systems/isystem.h"
#include "ui/ui_layer.h"

namespace PPGE
{
struct UISystemProps
{
};

class PPGE_API UISystem : public ISystem<UISystemProps>
{
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

    void PushFrontQueue(UILayer *subsystem);
    void PushBackQueue(UILayer *subsystem);
    void PopFrontQueue(UILayer *subsystem);
    void PopBackQueue(UILayer *subsystem);

    std::vector<UILayer *>::iterator begin()
    {
        return m_ManagedSubsytems.begin();
    }
    std::vector<UILayer *>::iterator end()
    {
        return m_ManagedSubsytems.end();
    }
    std::vector<UILayer *>::reverse_iterator rbegin()
    {
        return m_ManagedSubsytems.rbegin();
    }
    std::vector<UILayer *>::reverse_iterator rend()
    {
        return m_ManagedSubsytems.rend();
    }

    std::vector<UILayer *>::const_iterator begin() const
    {
        return m_ManagedSubsytems.begin();
    }
    std::vector<UILayer *>::const_iterator end() const
    {
        return m_ManagedSubsytems.end();
    }
    std::vector<UILayer *>::const_reverse_iterator rbegin() const
    {
        return m_ManagedSubsytems.rbegin();
    }
    std::vector<UILayer *>::const_reverse_iterator rend() const
    {
        return m_ManagedSubsytems.rend();
    }

    inline static UISystem &GetLayerSystem()
    {
        return s_instance;
    }

  private:
    std::vector<UILayer *> m_ManagedSubsytems;
    unsigned int m_InsertIndex = 0;

    static UISystem &s_instance;
};

} // namespace PPGE