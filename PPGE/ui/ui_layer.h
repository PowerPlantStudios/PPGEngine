#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "event/application_event.h"
#include "event/input_event.h"

namespace PPGE
{

class PPGE_API UILayer
{
  public:
    UILayer(const std::string &debug_name = "Subsystem") : m_debug_name(debug_name)
    {
    }
    virtual ~UILayer() = default;

    virtual void OnAttach()
    {
    }
    virtual void OnDetach()
    {
    }
    virtual void OnUpdate(float timestamp)
    {
    }
    virtual void OnRender()
    {
    }
    virtual void OnInputEvent(InputEvent &event)
    {
    }
    virtual void OnApplicationEvent(ApplicationEvent &event)
    {
    }

    const std::string &GetName() const
    {
        return m_debug_name;
    }

  protected:
    std::string m_debug_name;
};

} // namespace PPGE
