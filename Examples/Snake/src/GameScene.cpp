#include "GameScene.h"
#include "Settings.h"
#include "Components.h"
#include "Behaviours.h"
#include "Events.h"

Spritesheet* spritesheet;

static bool isOutsideBorders(const glm::vec3& position)
{
	return position.x < X0 || position.x > X0 + WIDTH - CELL_SIZE || position.y < Y0 || position.y > Y0 + HEIGHT - CELL_SIZE;
}

void GameScene::Load()
{
	Renderer2D::setClearColor(Colors::White);

	Entity borderEntities[4] = { AddEntity(), AddEntity(), AddEntity(), AddEntity() };
	for (Entity e : borderEntities)
		AddComponent<Renderer2DComponent>(e, BORDER_COLOR);

	borderEntities[0].AddComponent<Transform2DComponent>(X0 - BORDER, Y0, WIDTH + 2 * BORDER, -BORDER);
	borderEntities[1].AddComponent<Transform2DComponent>(X0 + WIDTH, Y0 - BORDER, BORDER, HEIGHT + 2 * BORDER);
	borderEntities[2].AddComponent<Transform2DComponent>(X0 + WIDTH + BORDER, Y0 + HEIGHT, -WIDTH - 2 * BORDER, BORDER);
	borderEntities[3].AddComponent<Transform2DComponent>(X0, Y0 + HEIGHT + BORDER, -BORDER, -HEIGHT - 2 * BORDER);

	Entity snakeHead = AddEntity();
	snakeHead.AddComponent<Transform2DComponent>(0, 0, CELL_SIZE, CELL_SIZE);
	snakeHead.AddComponent<SnakeHeadComponent>(SNAKE_START_LENGTH);
	snakeHead.AddComponent<Renderer2DComponent>(HEAD_COLOR);
	snakeHead.AddComponent<Direction2DComponent>(Direction2DComponent::Direction::Up);
	snakeHead.AddBehaviour<DirectionController>();
	snakeHead.AddBehaviour<SnakeHeadBehaviour>();

	Entity food = AddEntity();
	food.AddComponent<Transform2DComponent>(0, 0, CELL_SIZE, CELL_SIZE);
	food.AddComponent<FoodComponent>(snakeHead);
	food.AddComponent<Renderer2DComponent>(FOOD_COLOR);
	food.AddBehaviour<FoodBehaviour>();

	for (int i = 0; i < SNAKE_START_LENGTH; i++)
	{
		Entity tailElement = AddEntity();
		tailElement.AddComponent<Transform2DComponent>(0, 0, CELL_SIZE, CELL_SIZE);
		tailElement.AddComponent<SnakeTailComponent>(snakeHead, i + 1);
		tailElement.AddComponent<Renderer2DComponent>(TAIL_COLOR);
		tailElement.AddBehaviour<SnakeTailBehaviour>();
	}
}

void GameScene::OnInit()
{
	for (auto entity : ViewEntities<SnakeHeadComponent>())
		DispatchEvent(SnakeResetEvent{ ToEntity(entity) });
}

void GameScene::OnUpdate(float deltaTime)
{
	static float timer = 0;
	timer += deltaTime;
	if (timer > TIME_STEP)
	{
		timer -= TIME_STEP;

		for (auto [entity, component] : View<SnakeTailComponent>())
		{
			component->tailIndex++;
			if (component->tailIndex > GetComponent<SnakeHeadComponent>(component->snake)->snakeLength)
				RemoveEntity(entity);
		}
	}
}

void GameScene::PollCollisionEvents()
{
	for (auto [entity, component] : View<SnakeHeadComponent>())
	{
		auto* transform = GetComponent<Transform2DComponent>(entity);

		for (auto foodEntity : ViewEntities<FoodComponent>())
		{
			if (GetComponent<Transform2DComponent>(foodEntity)->position == transform->position)
				DispatchEvent(SnakeEatEvent{ entity, foodEntity });
		}

		for (auto tailEntity : ViewEntities<SnakeTailComponent>())
			if (GetComponent<Transform2DComponent>(tailEntity)->position == transform->position)
				DispatchEvent(SnakeResetEvent{ entity });

		if (isOutsideBorders(transform->position))
			DispatchEvent(SnakeResetEvent{ entity });
	}
}

