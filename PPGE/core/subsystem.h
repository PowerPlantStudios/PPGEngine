#pragma once

#include "core/defines.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"

namespace PPGE {

	class PPGE_API Subsystem
	{
	public:
		Subsystem(const std::string& debugName = "Subsystem");
		virtual ~Subsystem() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float timestamp) {}
		virtual void OnRender() {}
		virtual void OnInputEvent(InputEvent& event) {}
		virtual void OnApplicationEvent(ApplicationEvent& event) {}

		const std::string& GetName() const { return m_DebugName; }
	
	protected:
		std::string m_DebugName;
	};

}