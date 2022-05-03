#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

#include "editor.h"

namespace PPGE
{
class Editor : public Application
{
  public:
    Editor()
    {
    }

    ~Editor()
    {
    }

    void StartUp() override
    {
        Application::StartUp();
        CreateWidget<EditorWidget>();
        APP_INFO("Application is started up.");
    }

    void ShutDown() override
    {
        APP_INFO("Application is shut down.");
        Application::ShutDown();
    }
};
} // namespace PPGE

PPGE::Application *PPGE::CreateApplication()
{
    return new Editor();
}