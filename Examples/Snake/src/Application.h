#pragma once
#include <Sonic.h>
#include "GameScene.h"
#include "StartScene.h"
#include "Settings.h"

class Application : public Sonic::App
{
public:
	Application()
		: Sonic::App(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false) {}

private:
	Scene* OnInit() override
	{
		gameScene = new GameScene();
		startScene = new StartScene(gameScene);
		return startScene;
	}

	GameScene* gameScene;
	StartScene* startScene;
};
