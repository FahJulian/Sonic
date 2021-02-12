#pragma once
#include <Sonic.h>

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

	void OnRender() override {};

	void PollCollisionEvents() override;
};
