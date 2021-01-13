#pragma once
#include <ctime>
#include <cstdlib>
#include <Sonic.h>
#include "Events.h"
#include "Components.h"
#include "Settings.h"

class SnakeTailBehaviour : public Behaviour
{
	void OnInit() override
	{
		onReset = AddRemovableListener(this, &SnakeTailBehaviour::OnReset);
	}

	void OnReset(const SnakeResetEvent& e)
	{
		auto* component = GetComponent<SnakeTailComponent>();
		if (!e.snake == component->snake)
			return;

		if (component->snakeIndex > SNAKE_START_LENGTH)
		{
			RemoveEntity();
			return;
		}

		auto* transform = GetComponent<Transform2DComponent>();
		transform->position.x = static_cast<float>(SNAKE_START_X);
		transform->position.y = static_cast<float>(SNAKE_START_Y - component->snakeIndex * CELL_SIZE);
	}

	void OnDestroy() override
	{
		RemoveEventListener<SnakeResetEvent>(onReset);
	}

	Ref<int> onReset;
};


class FoodBehaviour : public Behaviour
{
	void OnInit() override
	{
		onReset = AddRemovableListener<SnakeResetEvent>([this](auto& e) {
			SONIC_LOG_DEBUG("FoodBevahious: onSnakeReset")
			if (e.snake == GetComponent<FoodComponent>()->snake)
				RandomizePosition();
			});

		onSnakeEat = AddRemovableListener<SnakeEatEvent>([this](auto& e) {
			SONIC_LOG_DEBUG("FoodBevahious: onSnakeEat")
			if (e.food == GetEntity())
				RandomizePosition();
			});
	}

	void RandomizePosition()
	{
		SONIC_LOG_DEBUG("Randomize")
		std::srand(static_cast<unsigned int>(std::time(0)));
		auto* transform = GetComponent<Transform2DComponent>();

		transform->position.x = static_cast<float>(X0 + (std::rand() % COLS) * CELL_SIZE);
		transform->position.y = static_cast<float>(Y0 + (std::rand() % ROWS) * CELL_SIZE);
	}

	void OnDestroy() override
	{
		RemoveEventListener<SnakeResetEvent>(onReset);
		RemoveEventListener<SnakeEatEvent>(onSnakeEat);
	}

	Ref<int> onReset;
	Ref<int> onSnakeEat;
};


class SnakeHeadBehaviour : public Behaviour
{
	void OnInit() override
	{
		onReset = AddRemovableListener(this, &SnakeHeadBehaviour::OnReset);
		onSnakeEat = AddRemovableListener<SnakeEatEvent>([this](auto& e) {
			SONIC_LOG_DEBUG("Snakle on eat")
			if (e.snake == GetEntity())
				GetComponent<SnakeHeadComponent>()->snakeLength++;
		});
	}

	void OnUpdate(float deltaTime) override
	{
		static float timer = 0;
		timer += deltaTime;
		if (timer > TIME_STEP)
		{
			timer -= TIME_STEP;

			auto* transform = GetComponent<Transform2DComponent>();
			auto* direction = GetComponent<DirectionComponent>();

			Entity newTailElement = GetScene()->AddEntity();
			GetScene()->AddComponent<Transform2DComponent>(newTailElement, *transform);
			GetScene()->AddComponent<SnakeTailComponent>(newTailElement, GetEntity(), 1);
			GetScene()->AddComponent<ColorComponent>(newTailElement, TAIL_COLOR);
			GetScene()->AddBehaviour<SnakeTailBehaviour>(newTailElement);

			SONIC_LOG_DEBUG("1")
			transform->position.x += static_cast<float>(static_cast<int>(direction->direction) % 2 * CELL_SIZE);
			SONIC_LOG_DEBUG("2")
			transform->position.y += static_cast<float>(static_cast<int>(direction->direction) / 2 * CELL_SIZE);
			SONIC_LOG_DEBUG("3")
			direction->lastDirection = direction->direction;
			SONIC_LOG_DEBUG("4")
		}
	}

	void OnReset(const SnakeResetEvent& e)
	{
		if (e.snake != GetEntity())
			return;

		GetComponent<SnakeHeadComponent>()->snakeLength = SNAKE_START_LENGTH;

		auto* t = GetComponent<Transform2DComponent>();
		t->position.x = SNAKE_START_X;
		t->position.y = SNAKE_START_Y;

		auto* d = GetComponent<DirectionComponent>();
		d->direction = DirectionComponent::Direction::Up;
		d->lastDirection = DirectionComponent::Direction::Up;
	}

	void OnDestroy() override
	{
		RemoveEventListener<SnakeEatEvent>(onSnakeEat);
		RemoveEventListener<SnakeResetEvent>(onReset);
	}

	Ref<int> onReset;
	Ref<int> onSnakeEat;
};


