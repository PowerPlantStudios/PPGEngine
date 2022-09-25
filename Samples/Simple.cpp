#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

constexpr float PI = 3.14159265359f;

namespace PPGE
{
class TestLayer : public Widget
{
  private:

  public:
    TestLayer() : Widget("TestSubsystem")
    {
    }

    void OnAttach() override
    {
    }

    void OnDetach() override
    {
    }

    void OnUpdate(float delta_time) override
    {
    }

    void OnRender() override
    {
    }

    virtual void OnImGui()
    {
    }
};

class Game : public ApplicationBase
{
  public:
    Game()
    {
    }

    ~Game()
    {
    }

    void StartUp() override
    {
        ApplicationBase::StartUp();
        CreateWidget<TestLayer>();
        APP_INFO("Application is started up.");
    }

    void ShutDown() override
    {
        APP_INFO("Application is shut down.");
        ApplicationBase::ShutDown();
    }
};
} // namespace PPGE

PPGE::ApplicationBase *PPGE::CreateApplication()
{
    return new PPGE::Game();
}
