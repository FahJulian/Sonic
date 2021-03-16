#include <Sonic.h>

using namespace Sonic;


void onEvent(const KeyPressedEvent& e)
{

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
		SONIC_LOG_DEBUG("TestScript updated");
	}

	Entity m_Entity;
	[[ transient ]] int i = 0;
};
