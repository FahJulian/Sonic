#include "LoadingScene.h"


class LoadingScript : public Script
{
	void OnUpdate(float deltaTime) override
	{
		m_Timer += deltaTime;
		if (m_Timer > 2.0f)
			SceneManager::setScene("gameScene");
	}

	float m_Timer = 0.0f;
};

void LoadingScene::OnLoad()
{
	Font font = Font(coreResourceDir() + "fonts/calibri.ttf", static_cast<int>(20.0f));

	Entity loader = AddEntity();
	AddComponent<ScriptComponent>(loader, new LoadingScript());

	Entity loadingText = AddEntity();
	AddComponent<UIComponent>(loadingText, UISize::Mode::WindowCenter, UISize::Mode::WindowCenter, font.StringWidth("Loading..."), font.StringHeight("Loading..."));
	AddComponent<UITextComponent>(loadingText, font, 0xe0e0e0ff, "Loading...");
}

void LoadingScene::OnInit()
{
	Window::setClearColor(0x1e150fff);
}
