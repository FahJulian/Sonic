#pragma once
#include <Sonic.h>

struct SnakeResetEvent
{
	EntityID snake;
};

struct SnakeEatEvent
{
	EntityID snake;
	EntityID food;
};
