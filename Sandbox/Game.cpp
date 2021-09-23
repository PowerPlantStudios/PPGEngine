#include <PPGE.h>

class TestLayer : public PPGE::UILayer
{
  public:
    TestLayer() : UILayer("TestSubsystem")
    {
    }

    void OnUpdate(float timestamp) override
    {
        // APP_INFO("Test system updated.");
    }

    void OnInputEvent(PPGE::InputEvent &inputEvent) override
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

        RegisterSubsystemToFrontQueue(new TestLayer());
    }

    ~Game()
    {
        APP_INFO("Application is destroyed.");
    }
};

PPGE::Application *PPGE::CreateApplication()
{
    return new Game();
}