#include "StartScene.h"
#include "Settings.h"

void StartScene::Load()
{
	Renderer2D::setClearColor(Colors::White);

	font = new Font(SONIC_RESOURCE_DIR + "fonts/calibri.ttf", 50);

	int buttonWidth = 200;
	int buttonHeight = 80;
	Entity button = UI::createButton(this, (WINDOW_WIDTH - buttonWidth) / 10.0f, (WINDOW_HEIGHT - buttonHeight) / 2.0f, 0.0f, (float)buttonWidth, (float)buttonHeight,
		Colors::LightGray, Colors::DarkGray, 1.0f, Colors::Black, 5.0f, [this](const MouseButtonReleasedEvent& e) { /*App::get()->SetScene(gameScene);*/ });
	button.AddComponent<TextComponent>(*font, Colors::Black, "Start");
	button.AddComponent<ResizableComponent>(UISize::Mode::Absolute, 0, 0, 500, 500, 5);

	Entity e1 = AddEntity();
	e1.AddComponent<UIComponent>(UISize::Mode::RelativeToWindow, 0.1f, 0.75f, 0.3f, 0.2f);
	e1.AddComponent<UIRendererComponent>(Colors::Orange);
	e1.AddComponent<ResizableComponent>(UISize::Mode::RelativeToWindow, 0, 0, 0.6f, 0.8f, 5);

	Entity e2 = AddEntity();
	e2.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.15f, 0.8f, 0.2f, 0.1f, e1);
	e2.AddComponent<UIRendererComponent>(Colors::DarkBlue);
	e2.AddComponent<ResizableComponent>(UISize::Mode::Absolute, 0, 0, 500, 500, 5);

	//for (int i = 0; i < 15990; i++)
	//{
		//Entity e = AddEntity();
		//e.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.25f, 0.25f, 0.5f, 0.5f, button);
		//e.AddComponent<UIRendererComponent>(Colors::Cyan);
		//e.AddComponent<ResizableComponent>(UISize::Mode::Absolute, 0, 0, 500, 500, 5);
	//}

}
