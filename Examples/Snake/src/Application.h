#pragma once
#include <Sonic.h>
#include "GameScene.h"
#include "Settings.h"

class Application : public Sonic::App
{
public:
	Application()
		: Sonic::App(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false) {}

private:
	Scene* OnInit() override
	{
		return new GameScene();
	}
};
