#include "GameScene.h"
#include "Settings.h"
#include "Components.h"
#include "Behaviours.h"
#include "Events.h"

Font* font;

static bool isOutsideBorders(const glm::vec3& position)
{
	return position.x < X0 || position.x > X0 + WIDTH - CELL_SIZE || position.y < Y0 || position.y > Y0 + HEIGHT - CELL_SIZE;
}

void GameScene::Load()
{
	Window::setClearColor(Colors::White);

	Entity borderEntities[4] = { AddEntity(), AddEntity(), AddEntity(), AddEntity() };
	for (Entity e : borderEntities)
		AddComponent<Renderer2DComponent>(e, BORDER_COLOR);

	AddComponent<Transform2DComponent>(borderEntities[0], X0 - BORDER, Y0, WIDTH + 2 * BORDER, -BORDER);
	AddComponent<Transform2DComponent>(borderEntities[1], X0 + WIDTH, Y0 - BORDER, BORDER, HEIGHT + 2 * BORDER);
	AddComponent<Transform2DComponent>(borderEntities[2], X0 + WIDTH + BORDER, Y0 + HEIGHT, -WIDTH - 2 * BORDER, BORDER);
	AddComponent<Transform2DComponent>(borderEntities[3], X0, Y0 + HEIGHT + BORDER, -BORDER, -HEIGHT - 2 * BORDER);

	Entity snakeHead = AddEntity();
	AddComponent<Transform2DComponent>(snakeHead, 0, 0, CELL_SIZE, CELL_SIZE);
	AddComponent<SnakeHeadComponent>(snakeHead, SNAKE_START_LENGTH);
	AddComponent<Renderer2DComponent>(snakeHead, HEAD_COLOR);
	AddComponent<Direction2DComponent>(snakeHead, Direction2DComponent::Direction::Up);
	//AddBehaviour<DirectionController>(snakeHead);
	//AddBehaviour<SnakeHeadBehaviour>(snakeHead);

	Entity food = AddEntity();
	AddComponent<Transform2DComponent>(food, 0, 0, CELL_SIZE, CELL_SIZE);
	AddComponent<FoodComponent>(food, snakeHead);
	AddComponent<Renderer2DComponent>(food, FOOD_COLOR);

	for (int i = 0; i < SNAKE_START_LENGTH; i++)
	{
		Entity tailElement = AddEntity();		
		AddComponent<Transform2DComponent>(tailElement, 0, 0, CELL_SIZE, CELL_SIZE);
		AddComponent<SnakeTailComponent>(tailElement, snakeHead, i + 1);
		AddComponent<Renderer2DComponent>(tailElement, TAIL_COLOR);
		//AddBehaviour<SnakeTailBehaviour>(tailElement);
	}
}

void GameScene::OnInit()
{
	for (auto entity : ViewEntities<SnakeHeadComponent>())
		DispatchEvent(SnakeResetEvent{ entity });
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
	//for (auto [entity, component, transform] : Group<SnakeHeadComponent, Transform2DComponent>())
	//{
	//	for (auto [foodEntity, f, t] : Group<FoodComponent, Transform2DComponent>())
	//		if (t->GetPosition() == transform->GetPosition())
	//			DispatchEvent(SnakeEatEvent{ entity, foodEntity });

	//	for (auto [tailEntity, st, t] : Group<SnakeTailComponent, Transform2DComponent>())
	//		if (t->GetPosition() == transform->GetPosition())
	//			DispatchEvent(SnakeResetEvent{ entity });

	//	if (isOutsideBorders(transform->GetPosition()))
	//		DispatchEvent(SnakeResetEvent{ entity });
	//}
}
