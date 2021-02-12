#pragma once
#include <Sonic.h>
#include "GameScene.h"

using namespace Sonic;


class StartScene : public Scene
{
public:
	StartScene(GameScene* gameScene)
		: gameScene(gameScene), font(nullptr) {}

private:
	void OnLoad() override;
	void OnInit() override;

	Font* font;
	GameScene* gameScene;
};
