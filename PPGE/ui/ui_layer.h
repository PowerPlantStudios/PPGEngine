#pragma once

#include "core/defines.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"

namespace PPGE
{

class PPGE_API UILayer
{
  public:
    UILayer(const std::string &debugName = "Subsystem") : m_DebugName(debugName)
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
        return m_DebugName;
    }

  protected:
    std::string m_DebugName;
};

} // namespace PPGE
