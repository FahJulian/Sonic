#pragma once
#include <Sonic.h>

using namespace Sonic;


class TestScript2 : public Script
{
	String c;
	int a;
	
public:
	void TestFunc(int i)
	{
		SONIC_LOG_DEBUG("TestScript2: ", i);
	}

	void OnClicked(const UIEntityClickedEvent& e)
	{
		SONIC_LOG_DEBUG("Clicked");
	}

	void OnInit() override
	{
		SONIC_LOG_DEBUG("TestScript2 initialized");
	}

private:
	static void test()
	{

	}

protected:
	static inline bool test2(int a)
	{
		return false;
	}
};
