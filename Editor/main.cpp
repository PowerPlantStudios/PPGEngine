#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

#include "editor.h"

namespace PPGE
{
class Editor : public ApplicationBase
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
        ApplicationBase::StartUp();
        CreateWidget<EditorWidget>();
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
    return new Editor();
}