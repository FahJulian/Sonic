#pragma once
#include <Sonic.h>
#include "GameScene.h"

using namespace Sonic;


class StartScene : public Scene
{
public:
	StartScene() = default;

private:
	void OnLoad() override;
	void OnInit() override;

	void OnKeyPressed(const KeyPressedEvent& e);
};
