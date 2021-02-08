#include "StartScene.h"
#include "Settings.h"

void StartScene::Load()
{
	Window::setClearColor(Colors::White);

	font = new Font(coreResourceDir() + "fonts/calibri.ttf", 50);

	int buttonWidth = 200;
	int buttonHeight = 80;
	Entity button = UI::createButton(this, (WINDOW_WIDTH - buttonWidth) / 10.0f, (WINDOW_HEIGHT - buttonHeight) / 2.0f, 0.0f, (float)buttonWidth, (float)buttonHeight,
		Colors::LightGray, Colors::DarkGray, 1.0f, Colors::Black, 5.0f, [this](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Clicked"); });
	//button.AddComponent<UITextComponent>(*font, Colors::Black, "Start");
	button.AddComponent<UIResizableComponent>([](const UIEntityResizedEvent& e) { SONIC_LOG_DEBUG(e.width, ", ", e.height); });
	button.AddComponent<UIMovableComponent>([](const UIEntityMovedEvent& e) { SONIC_LOG_DEBUG(e.deltaX, ", ", e.deltaY); }, StandardCursors::Link);
	button.AddComponent<UIPositionConstraintsComponent>(20.0f, 20.0f, 300.0f, 300.0f);
	button.AddComponent<UISizeConstraintsComponent>(20.0f, 20.0f, 150.0f, 150.0f);

	Entity e1 = AddEntity();
	e1.AddComponent<UIComponent>(UISize::Mode::RelativeToWindow, 0.1f, 0.75f, 0.3f, 0.2f);
	e1.AddComponent<UIRendererComponent>(UIRendererProperties(Colors::Orange));
	e1.AddComponent<UIResizableComponent>();
	e1.AddComponent<UIMovableComponent>(StandardCursors::Pen);

	Entity e2 = AddEntity();
	e2.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.15f, 0.8f, 0.2f, 0.1f, e1);
	e2.AddComponent<UIRendererComponent>(UIRendererProperties(Colors::DarkBlue));
	e2.AddComponent<UIResizableComponent>();

	Entity e3 = AddEntity(); 
	e3.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.25f, 0.25f, 0.5f, 0.5f, button);
	e3.AddComponent<UIRendererComponent>(UIRendererProperties(Colors::Cyan));
	e3.AddComponent<UIResizableComponent>();

	Entity e4 = AddEntity();
	e4.AddComponent<UIComponent>(UISize::Mode::RelativeToEntity, 0.25f, 0.25f, 0.5f, 0.5f, e3);
	e4.AddComponent<UIRendererComponent>(UIRendererProperties(Colors::Black));
}
