#include <array>
#include <cstdlib>
#include <ctime>
#include "Settings.h"
#include "GameScene.h"


static void getRandomPosition(int& row, int& col)
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    col = std::rand() % COLS;
    row = std::rand() % ROWS;
}

void GameScene::OnInit()
{
    Renderer2D::setClearColor(Colors::White);

    Window::onKeyReleased->AddListener<GameScene, &GameScene::OnKeyReleased>(this);

    Entity camera = AddEntity();
    AddComponent<Camera2DComponent>(camera, Camera2D(0.0f, Window::getWidth(), 0.0f, Window::getHeight()));
    AddComponent<Transform2DComponent>(camera, 0, 0, 0, 0);

    Reset();

    AddBorderEntities();
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
                    component.snakeSize++;
                    int row, col;
                    getRandomPosition(row, col);
                    ft->position.x = static_cast<float>(PADDING + BORDER_THICKNESS + col * SNAKE_ELEMENT_SIZE);
                    ft->position.y = static_cast<float>(PADDING + BORDER_THICKNESS + row * SNAKE_ELEMENT_SIZE);
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
            newPosition.x += static_cast<int>(component.direction) % 2 * SNAKE_ELEMENT_SIZE;
            newPosition.y += static_cast<int>(component.direction) / 2 * SNAKE_ELEMENT_SIZE;

            component.lastDirection = component.direction;

            if (IsOutOfBorders(newPosition))
            {
                Reset();
                return;
            }
            else
            {
                Entity newBodyPart = AddEntity();
                AddComponent<Transform2DComponent>(newBodyPart, t->position.x, t->position.y, SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE);
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

void GameScene::OnRender()
{
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

void GameScene::AddBorderEntities()
{
    std::array<Entity, 4> borderEntities{ AddEntity(), AddEntity(), AddEntity(), AddEntity() };
    for (Entity e : borderEntities)
        AddComponent<ColorComponent>(e, BORDER_COLOR);

    AddComponent<Transform2DComponent>(borderEntities[0],
        /* x */ PADDING + BORDER_THICKNESS - BORDER_THICKNESS, 
        /* y */ PADDING + BORDER_THICKNESS, 
        /* w */ COLS * SNAKE_ELEMENT_SIZE + 2 * BORDER_THICKNESS, 
        /* h */ -BORDER_THICKNESS
    );

    AddComponent<Transform2DComponent>(borderEntities[1],
        /* x */ PADDING + BORDER_THICKNESS, 
        /* y */ PADDING + BORDER_THICKNESS + ROWS * SNAKE_ELEMENT_SIZE + BORDER_THICKNESS,
        /* w */ -BORDER_THICKNESS,
        /* h */ -ROWS * SNAKE_ELEMENT_SIZE - 2 * BORDER_THICKNESS
    );

    AddComponent<Transform2DComponent>(borderEntities[2],
        /* x */ PADDING + BORDER_THICKNESS + COLS * SNAKE_ELEMENT_SIZE,
        /* y */ PADDING + BORDER_THICKNESS - BORDER_THICKNESS,
        /* w */ BORDER_THICKNESS,
        /* h */ ROWS * SNAKE_ELEMENT_SIZE + 2 * BORDER_THICKNESS
    );

    AddComponent<Transform2DComponent>(borderEntities[3],
        /* x */ PADDING + BORDER_THICKNESS + COLS * SNAKE_ELEMENT_SIZE + BORDER_THICKNESS,
        /* y */ PADDING + BORDER_THICKNESS + ROWS * SNAKE_ELEMENT_SIZE,
        /* w */ -COLS * SNAKE_ELEMENT_SIZE - 2 * BORDER_THICKNESS,
        /* h */ BORDER_THICKNESS
    );
}

bool GameScene::IsOutOfBorders(const glm::vec3& position)
{
    static const int minX = PADDING + BORDER_THICKNESS;
    static const int maxX = PADDING + BORDER_THICKNESS + (COLS - 1) * SNAKE_ELEMENT_SIZE;
    static const int minY = PADDING + BORDER_THICKNESS;
    static const int maxY = PADDING + BORDER_THICKNESS + (ROWS - 1) * SNAKE_ELEMENT_SIZE;

    return (position.x < minX || position.x > maxX || position.y < minY || position.y > maxY);
}

void GameScene::Reset()
{
    View<SnakeHeadComponent>().ForEach([this](auto e, auto& c) { RemoveEntity(e); });
    View<SnakeBodyPartComponent>().ForEach([this](auto e, auto& c) { RemoveEntity(e); });
    View<FoodComponent>().ForEach([this](auto e, auto& c) { RemoveEntity(e); });

    Entity snakeHead = AddEntity();
    AddComponent<Transform2DComponent>(snakeHead, PADDING + BORDER_THICKNESS + COLS / 2 * SNAKE_ELEMENT_SIZE,
        PADDING + BORDER_THICKNESS + ROWS / 2 * SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE);
    AddComponent<ColorComponent>(snakeHead, SNAKE_HEAD_COLOR);
    AddComponent<SnakeHeadComponent>(snakeHead);

    for (int i = 0; i < INTIAL_SNAKE_SIZE; i++)
    {
        Entity bodyPart = AddEntity();
        AddComponent<Transform2DComponent>(bodyPart, PADDING + BORDER_THICKNESS + COLS / 2 * SNAKE_ELEMENT_SIZE,
            PADDING + BORDER_THICKNESS + ROWS / 2 * SNAKE_ELEMENT_SIZE - (i + 1) * SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE);
        AddComponent<SnakeBodyPartComponent>(bodyPart, INTIAL_SNAKE_SIZE - 1 - i);
        AddComponent<ColorComponent>(bodyPart, SNAKE_BODY_COLOR);
    }

    int row, col;
    getRandomPosition(row, col);

    Entity food = AddEntity();
    AddComponent<Transform2DComponent>(food, PADDING + BORDER_THICKNESS + col * SNAKE_ELEMENT_SIZE, 
        PADDING + BORDER_THICKNESS + row * SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE, SNAKE_ELEMENT_SIZE);
    AddComponent<FoodComponent>(food);
    AddComponent<ColorComponent>(food, FOOD_COLOR);
}
