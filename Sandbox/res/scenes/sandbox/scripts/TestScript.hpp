#include <Sonic.h>

using namespace Sonic;


void onEvent(const KeyPressedEvent& e)
{
	SONIC_LOG_DEBUG("Key pressed");

	switch (e.key)
	{
	case Keys::F11:
		if (Window::getWindowMode() == WindowMode::Windowed)
		{
			Window::setWindowMode(WindowMode::WindowedBorderless);
		}
		else
		{
			Window::setWindowMode(WindowMode::Windowed);
		}
		break;
		break;
	}
}


class TestScript : public Script
{
public:
	TestScript() = default;

	TestScript(Entity entity)
		: m_Entity(entity)
	{
	}
	
	void OnInit() override
	{
		SONIC_LOG_DEBUG("TestScript initialized");
		EventDispatcher::addListener<KeyPressedEvent>(onEvent);
	}

	void OnClick()
	{
		SONIC_LOG_DEBUG("Clicked");
	}

	void OnUpdate(float deltaTime) override
	{
		SONIC_LOG_DEBUG(App::resourceDir());
	}

	Entity m_Entity = 0;
	[[ transient ]] int i = 0;
};
