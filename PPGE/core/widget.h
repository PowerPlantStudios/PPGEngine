#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "event/application_event.h"
#include "event/input_event.h"

namespace PPGE
{

class PPGE_API Widget
{
  public:
    Widget(const std::string &debug_name = "Subsystem");

    virtual ~Widget() = default;

    virtual void OnAttach()
    {
    }
    virtual void OnDetach()
    {
    }
    virtual void OnUpdate(float delta_time)
    {
    }
    virtual void OnRender()
    {
    }
    virtual void OnImGui()
    {
    }
    virtual void OnInputEvent(InputEvent &event)
    {
    }
    virtual void OnApplicationEvent(ApplicationEvent &event)
    {
    }

    inline const std::string &GetName() const
    {
        return m_debug_name;
    }

  protected:
    void Destroy();

    std::string m_debug_name;
};

} // namespace PPGE
