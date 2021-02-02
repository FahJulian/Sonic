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
		GetScene()->AddKeyedListener(this, &SnakeTailBehaviour::OnReset);
	}

	void OnReset(const SnakeResetEvent& e)
	{
		auto* component = GetEntity().GetComponent<SnakeTailComponent>();
		if (e.snake != component->snake)
			return;

		if (component->tailIndex > SNAKE_START_LENGTH)
		{
			GetScene()->RemoveEntity(GetEntity());
			return;
		}

		auto* transform = GetEntity().GetComponent<Transform2DComponent>();
		transform->SetX(static_cast<float>(SNAKE_START_X));
		transform->SetY(static_cast<float>(SNAKE_START_Y - component->tailIndex * CELL_SIZE));
	}

	void OnDestroy() override
	{
		GetScene()->RemoveKeyedListener<SnakeResetEvent>(this);
	}
};


class FoodBehaviour : public Behaviour
{
	void OnInit() override
	{
		GetScene()->AddKeyedListener(this, &FoodBehaviour::OnReset);
		GetScene()->AddKeyedListener(this, &FoodBehaviour::OnSnakeEat);
	}

	void OnReset(const SnakeResetEvent& e)
	{
		if (e.snake == GetEntity().GetComponent<FoodComponent>()->snake)
			RandomizePosition();
	}

	void OnSnakeEat(const SnakeEatEvent& e)
	{
		if (e.food == GetEntity())
			RandomizePosition();
	}

	void RandomizePosition()
	{
		std::srand(static_cast<unsigned int>(std::time(0)));
		auto* transform = GetEntity().GetComponent<Transform2DComponent>();

		transform->SetX(static_cast<float>(X0 + (std::rand() % COLS) * CELL_SIZE));
		transform->SetY(static_cast<float>(Y0 + (std::rand() % ROWS) * CELL_SIZE));
	}

	void OnDestroy() override
	{
		GetScene()->RemoveKeyedListener<SnakeResetEvent>(this);
		GetScene()->RemoveKeyedListener<SnakeEatEvent>(this);
	}
};


class SnakeHeadBehaviour : public Behaviour
{
	void OnInit() override
	{
		GetScene()->AddKeyedListener(this, &SnakeHeadBehaviour::OnReset);
		GetScene()->AddKeyedListener(this, &SnakeHeadBehaviour::OnSnakeEat); 
	}

	void OnUpdate(float deltaTime) override
	{
		static float timer = 0;
		timer += deltaTime;
		if (timer > TIME_STEP)
		{
			timer -= TIME_STEP;

			auto* transform = GetEntity().GetComponent<Transform2DComponent>();
			auto* direction = GetEntity().GetComponent<Direction2DComponent>();

			Entity newTailElement = GetScene()->AddEntity();
			newTailElement.AddComponent<Transform2DComponent>(*transform);
			newTailElement.AddComponent<SnakeTailComponent>(GetEntity(), 1);
			newTailElement.AddComponent<Renderer2DComponent>(TAIL_COLOR);
			newTailElement.AddBehaviour<SnakeTailBehaviour>();

			transform->SetX(transform->GetX() + static_cast<float>(static_cast<int>(direction->direction) % 2 * CELL_SIZE));
			transform->SetY(transform->GetY() + static_cast<float>(static_cast<int>(direction->direction) / 2 * CELL_SIZE));
			direction->lastDirection = direction->direction;
		}
	}

	void OnSnakeEat(const SnakeEatEvent& e)
	{
		if (e.snake == GetEntity())
			GetEntity().GetComponent<SnakeHeadComponent>()->snakeLength++;
	}

	void OnReset(const SnakeResetEvent& e)
	{
		if (e.snake != GetEntity())
			return;

		GetEntity().GetComponent<SnakeHeadComponent>()->snakeLength = SNAKE_START_LENGTH;

		auto* t = GetEntity().GetComponent<Transform2DComponent>();
		t->SetX(SNAKE_START_X);
		t->SetY(SNAKE_START_Y);

		auto* d = GetEntity().GetComponent<Direction2DComponent>();
		d->direction = Direction2DComponent::Direction::Up;
		d->lastDirection = Direction2DComponent::Direction::Up;
	}

	void OnDestroy() override
	{
		GetScene()->RemoveKeyedListener<SnakeEatEvent>(this);
		GetScene()->RemoveKeyedListener<SnakeResetEvent>(this);
	}
};


