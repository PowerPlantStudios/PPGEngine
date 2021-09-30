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
        if (PPGE::Input::IsKeyPressed(PPGE_KEY_E))
        {
            APP_TRACE("Key E is pressed");
        }
        if (PPGE::Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_0))
        {
            auto x_pose = PPGE::Input::GetMouseX();
            APP_TRACE("Mouse x - {0} ", x_pose);
        }
        if (PPGE::Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_1))
        {
            auto y_pose = PPGE::Input::GetMouseY();
            APP_TRACE("Mouse y - {0} ", y_pose);
        }
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

        PushLayerBack(std::make_unique<TestLayer>());
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