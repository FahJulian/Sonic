#include "StartScene.h"
#include "Settings.h"

void StartScene::Load()
{
	Window::setClearColor(Colors::DarkGray);
	Window::setSize(840, 480);

	Font font(coreResourceDir() + "fonts/calibri.ttf", 0.055f * Window::getHeight());

	EntityGroup startMenu = AddEntityGroup();
	Entity button;

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 240, 420, 40, UIRendererProperties(Colors::LightGray).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [=](const auto& e) { DeactivateEntities(startMenu); }, font, 0xbbbbbbff, "Singleplayer");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 190, 420, 40, UIRendererProperties(Colors::Red).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {}, font, 0xddddddff, "Multiplayer");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 140, 420, 40, UIRendererProperties(Colors::DarkBlue).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {}, font, 0xddddddff, "Minecraft Realms");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 150, 70, 40, 40, UIRendererProperties(Colors::Transparent).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {});
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 70, 200, 40, UIRendererProperties(Colors::Green).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {}, font, 0xddddddff, "Options...");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 430, 70, 200, 40, UIRendererProperties(Colors::Orange).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) { App::stop(); }, font, 0xddddddff, "Quit Game");
	AddToGroup(startMenu, button);

	//DeactivateEntity(100);
	//DeactivateEntity(3);
	//DeactivateEntity(2);

	//ReactivateEntity(5);
	//ReactivateEntity(3);
	//ReactivateEntity(2);
}
