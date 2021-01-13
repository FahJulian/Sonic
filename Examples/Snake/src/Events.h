#pragma once
#include <Sonic.h>

struct SnakeResetEvent
{
	SONIC_REGISTER_EVENT

	Entity snake;
};

struct SnakeEatEvent
{
	SONIC_REGISTER_EVENT

	Entity snake;
	Entity food;
};
