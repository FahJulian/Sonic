#include <array>
#include <cstdlib>
#include <ctime>
#include "Settings.h"
#include "GameScene.h"


void GameScene::OnInit()
{
    Renderer2D::setClearColor(Colors::White);

    AddEventListener(this, &GameScene::OnKeyReleased);

    Entity camera = AddEntity();
    AddComponent<Camera2DComponent>(camera, Camera2D(0.0f, Window::getWidth(), 0.0f, Window::getHeight()));
    AddComponent<Transform2DComponent>(camera, 0, 0, 0, 0);

    AddBorder();
    InitEntities();
}

void GameScene::OnUpdate(float deltaTime)
{
    static float timer = 0;

    timer += deltaTime;
    if (timer > TIME_STEP)
    {
        timer -= TIME_STEP;

        for (auto& [entity, component] : View<SnakeHeadComponent>())
        {
            auto* t = GetComponent<Transform2DComponent>(entity);

            for (auto& [food, foodComponent] : View <FoodComponent>())
            {
                auto* ft = GetComponent<Transform2DComponent>(food);
                if (ft->position == t->position)
                {
                    RandomizeFoodPos();
                    component.snakeSize++;
                }
            }

            for (auto& [bodyPart, bodyPartComponent] : View <SnakeBodyPartComponent>())
            {
                auto* bpt = GetComponent<Transform2DComponent>(bodyPart);
                if (bpt->position == t->position)
                {
                    Reset();
                    return;
                }
            }

            glm::vec3 newPosition = t->position;
            newPosition.x += static_cast<int>(component.direction) % 2 * ELEMENT_SIZE;
            newPosition.y += static_cast<int>(component.direction) / 2 * ELEMENT_SIZE;

            component.lastDirection = component.direction;

            if (IsOutOfBorders(newPosition))
            {
                Reset();
                return;
            }
            else
            {
                Entity newBodyPart = AddEntity();
                AddComponent<Transform2DComponent>(newBodyPart, t->position.x, t->position.y, ELEMENT_SIZE, ELEMENT_SIZE);
                AddComponent<ColorComponent>(newBodyPart, SNAKE_BODY_COLOR);
                AddComponent<SnakeBodyPartComponent>(newBodyPart, component.snakeSize);
                
                t->position = newPosition;
            }
        }

        for (auto& [entity, component] : View<SnakeBodyPartComponent>())
            if (component.lifetime-- == 0)
                RemoveEntity(entity);
    }
}

void GameScene::OnKeyReleased(const KeyReleasedEvent& event)
{
    switch (event.key)
    {
        case Keys::W:
        case Keys::Up:
            View<SnakeHeadComponent>().ForEach([](auto e, auto& c) { if (c.lastDirection != Direction::Down) c.direction = Direction::Up; });
            break;
        case Keys::A:
        case Keys::Left:
            View<SnakeHeadComponent>().ForEach([](auto e, auto& c) { if (c.lastDirection != Direction::Right) c.direction = Direction::Left; });
            break;
        case Keys::S:
        case Keys::Down:
            View<SnakeHeadComponent>().ForEach([](auto e, auto& c) { if (c.lastDirection != Direction::Up) c.direction = Direction::Down; });
            break;
        case Keys::D:
        case Keys::Right:
            View<SnakeHeadComponent>().ForEach([](auto e, auto& c) { if (c.lastDirection != Direction::Left) c.direction = Direction::Right; });
            break;

        default: 
            break;
    }
}

bool GameScene::IsOutOfBorders(const glm::vec3& position)
{
    static const int minX = PADDING + BORDER;
    static const int maxX = PADDING + BORDER + (COLS - 1) * ELEMENT_SIZE;
    static const int minY = PADDING + BORDER;
    static const int maxY = PADDING + BORDER + (ROWS - 1) * ELEMENT_SIZE;

    return (position.x < minX || position.x > maxX || position.y < minY || position.y > maxY);
}

void GameScene::AddBorder()
{
    std::array<Entity, 4> borderEntities{ AddEntity(), AddEntity(), AddEntity(), AddEntity() };
    for (Entity e : borderEntities)
        AddComponent<ColorComponent>(e, BORDER_COLOR);

    AddComponent<Transform2DComponent>(borderEntities[0], GRID_X0 - BORDER,
        GRID_Y0, GRID_WIDTH + 2 * BORDER, -BORDER);
    AddComponent<Transform2DComponent>(borderEntities[1], GRID_X0,
        GRID_Y0 + GRID_HEIGHT + BORDER, -BORDER, -GRID_HEIGHT - 2 * BORDER);
    AddComponent<Transform2DComponent>(borderEntities[2], GRID_X0 + GRID_WIDTH,
        GRID_Y0 - BORDER, BORDER, GRID_HEIGHT + 2 * BORDER);
    AddComponent<Transform2DComponent>(borderEntities[3], GRID_X0 + GRID_WIDTH + BORDER,
        GRID_Y0 + GRID_HEIGHT + BORDER, -GRID_WIDTH - 2 * BORDER, -BORDER);
}

void GameScene::InitEntities()
{
    Entity snakeHead = AddEntity();
    AddComponent<Transform2DComponent>(snakeHead, SNAKE_START_X, SNAKE_START_Y, ELEMENT_SIZE, ELEMENT_SIZE);
    AddComponent<ColorComponent>(snakeHead, SNAKE_HEAD_COLOR);
    AddComponent<SnakeHeadComponent>(snakeHead);

    for (int i = 0; i < INTIAL_SNAKE_SIZE; i++)
    {
        Entity bodyPart = AddEntity();
        AddComponent<Transform2DComponent>(bodyPart, SNAKE_START_X, SNAKE_START_Y - (i + 1) * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE);
        AddComponent<SnakeBodyPartComponent>(bodyPart, INTIAL_SNAKE_SIZE - 1 - i);
        AddComponent<ColorComponent>(bodyPart, SNAKE_BODY_COLOR);
    }

    Entity food = AddEntity();
    AddComponent<Transform2DComponent>(food, 0, 0, ELEMENT_SIZE, ELEMENT_SIZE);
    AddComponent<FoodComponent>(food);
    AddComponent<ColorComponent>(food, FOOD_COLOR);

    RandomizeFoodPos();
}

void GameScene::RandomizeFoodPos()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (auto& [e, c] : View<FoodComponent>())
    {
        Transform2DComponent* t = GetComponent<Transform2DComponent>(e);
        t->position.x = static_cast<float>(GRID_X0 + (std::rand() % COLS) * ELEMENT_SIZE);
        t->position.y = static_cast<float>(GRID_Y0 + (std::rand() % ROWS) * ELEMENT_SIZE);
    }
}

void GameScene::Reset()
{
    View<SnakeHeadComponent>().ForEach([this](auto e, auto& c) { 
        auto* t = GetComponent<Transform2DComponent>(e); 
        t->position.x = SNAKE_START_X;
        t->position.y = SNAKE_START_Y;
        c.direction = Direction::Up;
        c.lastDirection = Direction::Up;
    });

    View<SnakeBodyPartComponent>().ForEach([this](auto e, auto& c) { RemoveEntity(e); });
    for (int i = 0; i < INTIAL_SNAKE_SIZE; i++)
    {
        Entity bodyPart = AddEntity();
        AddComponent<Transform2DComponent>(bodyPart, SNAKE_START_X, SNAKE_START_Y - (i + 1) * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE);
        AddComponent<SnakeBodyPartComponent>(bodyPart, INTIAL_SNAKE_SIZE - 1 - i);
        AddComponent<ColorComponent>(bodyPart, SNAKE_BODY_COLOR);
    }

    RandomizeFoodPos();
}
