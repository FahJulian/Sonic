#pragma once
#include <Sonic.h>
#include "Settings.h"

enum class Direction : char
{
    Right = 1,
    Left = -1,
    Up = 2,
    Down = -2
};

struct SnakeHeadComponent
{
    SONIC_REGISTER_COMPONENT(0)

    Direction lastDirection;
    Direction direction;
    int snakeSize;

    SnakeHeadComponent()
        : lastDirection(Direction::Up), direction(Direction::Up), snakeSize(INTIAL_SNAKE_SIZE)
    {
    }
};

struct SnakeBodyPartComponent
{
    SONIC_REGISTER_COMPONENT(1)

    int lifetime;

    SnakeBodyPartComponent(int lifetime)
        : lifetime(lifetime)
    {
    }
};

struct FoodComponent
{
    SONIC_REGISTER_COMPONENT(0)
};

class GameScene : public Scene
{
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnKeyReleased(const KeyReleasedEvent& event);
    void AddBorderEntities();
    bool IsOutOfBorders(const glm::vec3& position);
    void Reset();
};
