#pragma once
#include <Sonic.h>

struct SnakeResetEvent
{
	Entity snake;
};

struct SnakeEatEvent
{
	Entity snake;
	Entity food;
};
