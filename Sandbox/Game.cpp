#include <PPGE.h>

class TestSubsystem : public PPGE::Subsystem
{
public:
	TestSubsystem()
		: Subsystem("TestSubsystem")
	{
	}

	void OnUpdate(float timestamp) override
	{
		// APP_INFO("Test system updated.");
	}

	void OnInputEvent(PPGE::InputEvent& inputEvent) override
	{
		// APP_INFO("Input event is received by the test system.");
	}

};

class Game : public PPGE::Application
{
public:
	Game()
	{
		APP_INFO("Application is initialized.");
		
		RegisterSubsystemToFrontQueue(new TestSubsystem());
	}

	~Game()
	{
		APP_INFO("Application is destroyed.");
	}


};


PPGE::Application* PPGE::CreateApplication()
{
	return new Game();
}