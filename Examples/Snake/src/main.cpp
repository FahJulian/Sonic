#pragma once
#include "Sonic/main.h"
#include "StartScene.h"
#include "GameScene.h"
#include "Settings.h"

Sonic::AppData Sonic::init()
{
	GameScene* gameScene = new GameScene();
	StartScene* startScene = new StartScene(gameScene);

	return {
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		true,
		startScene
	};
}

const std::string& Sonic::resourceDir()
{
	static const std::string RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
	return RESOURCE_DIR;
}
