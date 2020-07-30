#include <PPGE.h>

class Game : public PPGE::Application
{
public:
	Game()
	{
		APP_INFO("Application is initialized.");
		PPGE::MouseMovedEvent e(25.7f, 46.7f);
		
		if (e.IsInCategory(PPGE::InputEventCategoryBit::Mouse))
		{
			APP_INFO("Mouse input event is receieved.");
		}

		APP_INFO(e.ToString());
	}

	~Game()
	{

	}


};


PPGE::Application* PPGE::CreateApplication()
{
	return new Game();
}