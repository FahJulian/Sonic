#include "Sonic/main.h"
#include "StartScene.h"
#include "GameScene.h"
#include "LoadingScene.h"
#include "Settings.h"

Sonic::String Sonic::init()
{
	SceneManager::registerScene(new StartScene(), "startScene");
	SceneManager::registerScene(new LoadingScene(), "loadingScene");
	SceneManager::registerScene(new GameScene(), "gameScene");
	SceneManager::setScene("startScene");

	return "windows/sandbox";
}

const String& Sonic::resourceDir()
{
	static const String RESOURCE_DIR = "C:/dev/Sonic/Examples/Snake/res/";
	return RESOURCE_DIR;
}
