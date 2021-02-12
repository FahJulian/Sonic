#pragma once
#include "Sonic/main.h"
#include "StartScene.h"
#include "GameScene.h"
#include "Settings.h"

Sonic::AppData Sonic::init()
{
	GameScene* gameScene = new GameScene();
	StartScene* startScene = new StartScene();

	SceneManager::registerScene(gameScene, "gameScene");
	SceneManager::registerScene(startScene, "startScene");
	SceneManager::setScene("startScene");

	return {
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		true,
	};
}

const String& Sonic::resourceDir()
{
	static const String RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
	return RESOURCE_DIR;
}
