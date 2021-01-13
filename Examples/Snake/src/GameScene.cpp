#include "GameScene.h"
#include "Settings.h"
#include "Components.h"
#include "Behaviours.h"
#include "Events.h"

static bool isOutsideBorders(const glm::vec3& position)
{
	return position.x < X0 || position.x > X0 + WIDTH - CELL_SIZE || position.y < Y0 || position.y > Y0 + HEIGHT - CELL_SIZE;
}

void GameScene::OnInit()
{
	Renderer2D::setClearColor(Colors::White);

	Entity borderEntities[4] = { AddEntity(), AddEntity(), AddEntity(), AddEntity() };
	for (Entity e : borderEntities)
		AddComponent<ColorComponent>(e, BORDER_COLOR);

	AddComponent<Transform2DComponent>(borderEntities[0], X0 - BORDER, Y0, WIDTH + 2 * BORDER, -BORDER);
 	AddComponent<Transform2DComponent>(borderEntities[1], X0 + WIDTH, Y0 - BORDER, BORDER, HEIGHT + 2 * BORDER);
	AddComponent<Transform2DComponent>(borderEntities[2], X0 + WIDTH + BORDER, Y0 + HEIGHT, -WIDTH - 2 * BORDER, BORDER);
	AddComponent<Transform2DComponent>(borderEntities[3], X0, Y0 + HEIGHT + BORDER, -BORDER, - HEIGHT - 2 * BORDER);

	Entity snakeHead = AddEntity();
	AddComponent<Transform2DComponent>(snakeHead, 0, 0, CELL_SIZE, CELL_SIZE);
	AddComponent<SnakeHeadComponent>(snakeHead, SNAKE_START_LENGTH);
	AddComponent<ColorComponent>(snakeHead, HEAD_COLOR);
	AddComponent<DirectionComponent>(snakeHead, DirectionComponent::Direction::Up);
	AddBehaviour<DirectionController>(snakeHead);
	AddBehaviour<SnakeHeadBehaviour>(snakeHead);

	Entity food = AddEntity();
	AddComponent<Transform2DComponent>(food, 0, 0, CELL_SIZE, CELL_SIZE);
	AddComponent<FoodComponent>(food, snakeHead);
	AddComponent<ColorComponent>(food, FOOD_COLOR);
	AddBehaviour<FoodBehaviour>(food);

	for (int i = 0; i < SNAKE_START_LENGTH; i++)
	{
		Entity tailElement = AddEntity();
		AddComponent<Transform2DComponent>(tailElement, 0, 0, CELL_SIZE, CELL_SIZE);
		AddComponent<SnakeTailComponent>(tailElement, snakeHead, i);
		AddComponent<ColorComponent>(tailElement, TAIL_COLOR);
		AddBehaviour<SnakeTailBehaviour>(tailElement);
	}

	DispatchEvent(SnakeResetEvent{ snakeHead });
}

void GameScene::OnUpdate(float deltaTime)
{
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);
	//AddComponent<Transform2DComponent>(AddEntity(), 1, 1, 1, 1);

	static float timer = 0;
	timer += deltaTime;
	if (timer > TIME_STEP)
	{
		timer -= TIME_STEP;

		for (auto& [entity, component] : View<SnakeTailComponent>())
		{
			component.snakeIndex++;
			if (component.snakeIndex > GetComponent<SnakeHeadComponent>(component.snake)->snakeLength)
				RemoveEntity(entity);
		}
	}
}

void GameScene::CheckCollisions()
{
	for (auto& [entity, component] : View<SnakeHeadComponent>())
	{
		auto* transform = GetComponent<Transform2DComponent>(entity);

		for (auto& [foodEntity, FoodComponent] : View<FoodComponent>())
			if (GetComponent<Transform2DComponent>(foodEntity)->position == transform->position)
				DispatchEvent(SnakeEatEvent{ entity, foodEntity });

		for (auto& [tailEntity, tailComponent] : View<SnakeTailComponent>())
			if (GetComponent<Transform2DComponent>(tailEntity)->position == transform->position)
				DispatchEvent(SnakeResetEvent{ entity });

		if (isOutsideBorders(transform->position))
			DispatchEvent(SnakeResetEvent{ entity });
	}
}

