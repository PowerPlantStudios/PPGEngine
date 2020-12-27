#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/subsystem.h"
#include "core/input/input_event.h"

namespace PPGE
{
	class PPGE_API ImGuiSubsystem : public Subsystem
	{
	public:
		ImGuiSubsystem();
		~ImGuiSubsystem();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float timestamp) override;
		void OnInputEvent(InputEvent&) override;
	
	private:

	};
}