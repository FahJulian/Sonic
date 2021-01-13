#pragma once
#include <Sonic.h>

struct SnakeHeadComponent
{
	SONIC_REGISTER_COMPONENT(0)

	int snakeLength;

	SnakeHeadComponent(int snakeLength)
		: snakeLength(snakeLength) {}
};

struct SnakeTailComponent
{
	SONIC_REGISTER_COMPONENT(0)

	Entity snake;
	int snakeIndex;

	SnakeTailComponent(Entity snake, int snakeIndex = 1)
		: snake(snake), snakeIndex(snakeIndex) {}
};

struct FoodComponent
{
	SONIC_REGISTER_COMPONENT(0)

	Entity snake;

	FoodComponent(Entity snake)
		: snake(snake) {}
};
