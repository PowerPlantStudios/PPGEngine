#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

constexpr float PI = 3.14159265359f;

namespace PPGE
{
class TestLayer : public Widget
{
  private:
    ResourceManager m_resource_mgr;

  public:
    TestLayer() : Widget("TestSubsystem")
    {
        m_resource_mgr.RegisterLoaderMultiple<LazyResource, LazyLoader>({".txt"});
    }

    void OnAttach() override
    {
        m_resource_mgr.WalkRoot("C:/Users/Yagili/Documents/Workspace/TEST");
        m_resource_mgr.ConnectFileSystemObserver(
            [](const std::filesystem::path &path, FileSystemObserver::FileAction action) 
            {
                APP_INFO("Received file system observer event.");
                switch (action)
                {
                case PPGE::FileSystemObserver::FileAction::UNKNOWN:
                    break;
                case PPGE::FileSystemObserver::FileAction::ADDED:
                    APP_INFO("FileAction::ADDED : {0}", path.string());
                    break;
                case PPGE::FileSystemObserver::FileAction::MODIFIED:
                    APP_INFO("FileAction::MODIFIED : {0}", path.string());
                    break;
                case PPGE::FileSystemObserver::FileAction::REMOVED:
                    APP_INFO("FileAction::REMOVED : {0}", path.string());
                    break;
                case PPGE::FileSystemObserver::FileAction::RENAMED_FROM:
                    APP_INFO("FileAction::RENAMED_FROM : {0}", path.string());
                    break;
                case PPGE::FileSystemObserver::FileAction::RENAMED_TO:
                    APP_INFO("FileAction::RENAMED_TO : {0}", path.string());
                    break;
                default:
                    break;
                }
            });
    }

    void OnDetach() override
    {
        m_resource_mgr.DisconnectFileSystemObserver();
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
