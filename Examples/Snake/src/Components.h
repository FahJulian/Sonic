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
	int tailIndex;

	SnakeTailComponent(Entity snake, int tailIndex = 1)
		: snake(snake), tailIndex(tailIndex) {}
};

struct FoodComponent
{
	Entity snake;

	FoodComponent(Entity snake)
		: snake(snake) {}
};
