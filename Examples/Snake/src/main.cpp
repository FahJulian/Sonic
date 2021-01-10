#include <Sonic/main.h>
#include "GameScene.h"
#include "Settings.h"

class SnakeApp : public Sonic::App
{
    SnakeApp()
        : Sonic::App(WIDTH, HEIGHT, TITLE, false)
    {
    }

    Sonic::Scene* OnInit() override
    {
        std::cout << "Test 1" << std::endl;
        return new GameScene();
    }

    friend Sonic::App* Sonic::createApplication();
};

Sonic::App* Sonic::createApplication()
{
    return new SnakeApp();
}

const std::string Sonic::resourceDir()
{
    return "C:\\Users\\fahju\\dev\\cpp\\Sonic\\ExampleGames\\Snake\\res\\";
}
