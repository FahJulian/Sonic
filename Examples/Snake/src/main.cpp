#include "Sonic/main.h"
#include "StartScene.h"
#include "GameScene.h"
#include "Settings.h"

Sonic::AppData Sonic::init()
{
	SceneManager::registerScene(new StartScene(), "startScene");
	SceneManager::registerScene(new GameScene(), "gameScene");
	SceneManager::setScene("startScene");

	return {
		WindowMode::Windowed,
		WINDOW_TITLE,
		Colors::DarkGray,
		true,
		840,
		480,
	};
}

const String& Sonic::resourceDir()
{
	static const String RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
	return RESOURCE_DIR;
}
