#include "Application.h"

Scene* Application::OnInit()
{
	gameScene = new GameScene();
	startScene = new StartScene(gameScene);
	return startScene;
}
