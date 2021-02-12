#pragma once
#include <Sonic.h>

using namespace Sonic;


class GameScene : public Scene
{
public:
	GameScene()
		: Scene()
	{
	}

protected:
	void OnLoad() override;

	void OnInit() override;

	void OnUpdate(float deltaTime) override;

	void PollCollisionEvents() override;
};
