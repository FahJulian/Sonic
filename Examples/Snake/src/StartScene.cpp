#include "StartScene.h"
#include "Settings.h"

void StartScene::Load()
{
	Renderer2D::setClearColor(Colors::White);

	font = new Font(SONIC_RESOURCE_DIR + "fonts/calibri.ttf", 50);

	int buttonWidth = 200;
	int buttonHeight = 80;
	Entity button = UI::createButton(this, (WINDOW_WIDTH - buttonWidth) / 2.0f, (WINDOW_HEIGHT - buttonHeight) / 2.0f, 0.0f, (float)buttonWidth, (float)buttonHeight,
		Colors::LightGray, Colors::DarkGray, 1.0f, Colors::Black, 5.0f, [this](const MouseButtonReleasedEvent& e) { App::get()->SetScene(gameScene); });
	button.AddComponent<TextComponent>(*font, "Start", Colors::Black);
}
