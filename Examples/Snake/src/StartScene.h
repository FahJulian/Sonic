#pragma once
#include <Sonic.h>
#include "GameScene.h"

class StartScene : public Scene
{
public:
	StartScene(GameScene* gameScene)
		: gameScene(gameScene), spritesheet(nullptr) {}

private:
	void Load() override
	{
		Renderer2D::setClearColor(Colors::White);

		spritesheet = new Spritesheet(resourceDir() + "textures\\mario_sheet.png", 1, 14, 16, 16, 1);

		Entity startButton = AddEntity();
		startButton.AddComponent<Transform2DComponent>((Window::getWidth() - 100) / 2, (Window::getHeight() - 40) / 2, 100, 40);
		startButton.AddComponent<Renderer2DComponent>(Colors::LightGray);
	}

	GameScene* gameScene;
	Spritesheet* spritesheet;
};
