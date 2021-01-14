#pragma once
#include <Sonic.h>

struct SnakeHeadComponent
{
	int snakeLength;

	SnakeHeadComponent(int snakeLength)
		: snakeLength(snakeLength) {}
};

struct SnakeTailComponent
{
	Entity snake;
	int snakeIndex;

	SnakeTailComponent(Entity snake, int snakeIndex = 1)
		: snake(snake), snakeIndex(snakeIndex) {}
};

struct FoodComponent
{
	Entity snake;

	FoodComponent(Entity snake)
		: snake(snake) {}
};
