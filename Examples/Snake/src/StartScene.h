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
		startButton.AddComponent<HoverComponent>(Colors::DarkGray);
		startButton.AddComponent<BorderComponent>(Colors::Black, 1.0f);

		startButton.AddComponent<ClickListenerComponent>([this](const MouseButtonReleasedEvent& e) { App::get()->SetScene(gameScene); });
	}

	GameScene* gameScene;
	Spritesheet* spritesheet;
};
