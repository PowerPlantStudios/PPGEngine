#include "PPGEpch.h"

#include "imgui_subsytem.h"

#include "core/application.h"

namespace PPGE
{
	ImGuiSubsystem::ImGuiSubsystem()
		: Subsystem("ImGuiSubsystem")
	{
	}

	ImGuiSubsystem::~ImGuiSubsystem()
	{
	}

	void ImGuiSubsystem::OnAttach()
	{
	}

	void ImGuiSubsystem::OnDetach()
	{
	}
	
	void ImGuiSubsystem::OnUpdate(float timestamp)
	{
	}
	
	void ImGuiSubsystem::OnInputEvent(InputEvent&)
	{
	}
}