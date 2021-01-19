#pragma once
#include <Sonic.h>
#include "GameScene.h"

class StartScene : public Scene
{
public:
	StartScene(GameScene* gameScene)
		: gameScene(gameScene), font(nullptr) {}

private:
	void Load() override;

	Font* font;
	GameScene* gameScene;
};
