#include <functional>
#include "StartScene.h"
#include "Settings.h"
#include "Sonic/Scene/UI/UIUtils.h"

static Entity startPanel;
static Entity optionsPanel;


class Animation : public Script
{
public:
	Animation(EntityGroup group, float duration, float maxOffset)
		: m_Group(group), m_Duration(duration), m_MaxOffset(maxOffset)
	{
	}

	void Start()
	{
		if (!m_Running)
		{
			m_Running = true;
			m_Timer = 0;
			m_LastOffset = 0.0f;
		}
	}

	void OnUpdate(float deltaTime) override
	{
		if (m_Running)
		{
			m_Timer += deltaTime;
			if (m_Timer >= m_Duration)
			{
				m_Timer = m_Duration;
				m_Running = false;
			}

			float offset = (m_MaxOffset / (m_Duration * m_Duration)) * (-(2 * m_Timer - m_Duration) * (2 * m_Timer - m_Duration) + m_Duration * m_Duration);

			for (Entity entity : *m_Scene->GetGroup(m_Group))
			{
				UIComponent* c = m_Scene->GetComponent<UIComponent>(entity);
				UI::setX(m_Scene, entity, c, c->GetX() - (offset - m_LastOffset));
			}

			m_LastOffset = offset;
		}
	}

	EntityGroup m_Group;
	float m_Duration;
	float m_MaxOffset;
	float m_Timer = 0.0f;
	float m_LastOffset = 0.0f;
	bool m_Running = false;
};

class OptionsMenuScript : public Script
{
public:
	OptionsMenuScript(EntityGroup mainMenu, EntityGroup optionsMenu)
		: m_MainMenu(mainMenu), m_OptionsMenu(optionsMenu)
	{
	}

	void OnInit() override
	{
		EventDispatcher::addRemovableListener(this, &OptionsMenuScript::OnKeyPressed);
	}

	void OnKeyPressed(const KeyPressedEvent& e)
	{
		if (e.key == Keys::Escape)
		{
			m_Scene->DeactivateEntities(m_OptionsMenu);
			m_Scene->ReactivateEntities(m_MainMenu);
		}
	}

	void OnDestroy() override
	{
		EventDispatcher::removeListener<KeyPressedEvent>(this);
	}

	EntityGroup m_MainMenu;
	EntityGroup m_OptionsMenu;
};

class S1 : public Script
{
	void OnUpdate(float deltaTime) override
	{
		SONIC_LOG_DEBUG("Script 1 ", m_Entity);
	}
};

class S2 : public Script
{
	void OnUpdate(float deltaTime) override
	{
		SONIC_LOG_DEBUG("Script 2 ", m_Entity);
	}
};

void onMouseMoved(const MouseMovedEvent& e)
{
	SONIC_LOG_DEBUG("(", e.x, ", ", e.y, ")");
}

void StartScene::OnLoad()
{
	Font font  = Font(coreResourceDir() + "fonts/calibri.ttf", static_cast<int>(0.055f * Window::getHeight()));
	Texture backgroundTexture = Texture(resourceDir() + "textures/background.png");

	const UIRendererProperties buttonNormal = UIRendererProperties(0x6f6f6fff).Border(0x222222ff, 2).EdgeRadius(3);
	const UIRendererProperties buttonHovered = UIRendererProperties(0x7d87beff).Border(0x222222ff, 2).EdgeRadius(3);

	Entity background = AddEntity();
	AddComponent<UIComponent>(background, UISize::Mode::RelativeToWindow, 0.0f, 0.0f, 1.0f, 1.0f);
	AddComponent<UIRendererComponent>(background, UIRendererProperties(Sprite(backgroundTexture)));
	Entity backgroundBlur = AddEntity();
	AddComponent<UIComponent>(backgroundBlur, UISize::Mode::RelativeToWindow, 0.0f, 0.0f, 1.0f, 1.0f);
	AddComponent<UIRendererComponent>(backgroundBlur, UIRendererProperties(0xbbbbbb55));

	startPanel = AddEntityGroup();
	AddComponent<UIComponent>(startPanel, UISize{ UISize::Mode::WindowCenter, 0.0f }, UISize{ UISize::Mode::WindowCenter, -30.0f }, 400.0f, 200.0f);

	optionsPanel = AddEntityGroup();
	AddComponent<ScriptComponent>(optionsPanel, new OptionsMenuScript(startPanel, optionsPanel));

	UI::createButton(this, UISize::Mode::RelativeToEntity, 0.0f, 1.0f - 0.18f, 1.0f, 0.18f, buttonNormal, buttonHovered,
		[](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Singleplayer"); SceneManager::setScene("loadingScene"); }, font, 0xe0e0e0ff, "Singleplayer", startPanel);

	UI::createButton(this, UISize::Mode::RelativeToEntity, 0.0f, 1.0f - 2 * 0.18f - 0.05f, 1.0f, 0.18f, buttonNormal, buttonHovered,
		[](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Multiplayer"); }, font, 0xe0e0e0ff, "Multiplayer", startPanel);

	UI::createButton(this, UISize::Mode::RelativeToEntity, 0.0f, 1.0f - 3 * 0.18f - 2 * 0.05f, 1.0f, 0.18f, buttonNormal, buttonHovered,
		[](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Minecraft Realms"); }, font, 0xe0e0e0ff, "Minecraft Realms", startPanel);

	UI::createButton(this, UISize::Mode::RelativeToEntity, 0.0f, 0.0f, (1.0f - 0.025f) / 2, 0.18f, buttonNormal, buttonHovered,
		[=](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Options"); 
	DeactivateEntities(startPanel), 
		ReactivateEntities(optionsPanel); }, font, 0xe0e0e0ff, "Options", startPanel);

	UI::createButton(this, UISize::Mode::RelativeToEntity, 1.0f - (1.0f - 0.025f) / 2, 0.0f, (1.0f - 0.0275f) / 2, 0.18f, buttonNormal, buttonHovered,
		[](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Quit Game"); App::stop(); }, font, 0xe0e0e0ff, "Quit Game", startPanel);

	UI::createButton(this, UISize::Mode::RelativeToEntity, -0.15f, 0.0f, 0.09f, 0.18f, buttonNormal, buttonHovered,
		[](const MouseButtonReleasedEvent& e) { SONIC_LOG_DEBUG("Language"); }, startPanel);

	Entity optionsText = AddEntity(optionsPanel);
	AddComponent<UIComponent>(optionsText,
		UISize{ UISize::Mode::RelativeToWindow, 0.4f },
		UISize{ UISize::Mode::RelativeToWindow, 0.9f },
		UISize{ UISize::Mode::RelativeToWindow, 0.2f },
		UISize{ UISize::Mode::Absolute, 0.1f }
	);
	AddComponent<UITextComponent>(optionsText, font, Colors::White, "Options");

	//Entity test = AddEntity();
	//AddComponent<UIRendererComponent>(test, UIRendererProperties(Colors::Orange));
	//AddComponent<UIComponent>(test,
	//	UISize{ UISize::Mode::WindowCenter, 0.0f },
	//	UISize{ UISize::Mode::WindowCenter, 0.0f },
	//	UISize{ UISize::Mode::Absolute, 400 },
	//	UISize{ UISize::Mode::Absolute, 300 }
	//);

	//Entity fovSlider = AddEntity(optionsMenu);
	//AddComponent<UIComponent>(fovSlider,
	//	UISize{ UISize::Mode::WindowCenter, 0.35f },
	//	UISize{ UISize::Mode::RelativeToWindow, 0.75f },
	//	UISize{ UISize::Mode::RelativeToWindow, 0.3f },
	//	UISize{ UISize::Mode::Absolute, 40 }
	//);
	//AddComponent<UIRendererComponent>(fovSlider, UIRendererProperties(Colors::DarkGray).Border(Colors::Black, 2));

	//Entity slider = AddEntity(optionsMenu);
	//AddComponent<UIComponent>(slider, UISize::Mode::RelativeToEntity, (1.0f - 0.08f) / 2.0f, 0.0f, 0.08f, 1.0f, fovSlider);
	//AddComponent<UIRendererComponent>(slider, UIRendererProperties(Colors::LightGray).Border(Colors::Black, 2));
	//AddComponent<UIPositionConstraintsComponent>(slider, 0.0f, 0.0f, 1.0f - 0.08f, 0.0f);

	//EventListener<UISliderEvent> onSlide = [=](const UISliderEvent& e) {
	//	SONIC_LOG_DEBUG("Fov: ", (int)(80 * e.percentage + 30));
	//};

	//AddComponent<UIMovableComponent>(slider, [this, slider, fovSlider, onSlide](const UIEntityMovedEvent& e) {
	//	auto* sliderComponent = GetComponent<UIComponent>(slider);
	//	auto* backgroundComponent = GetComponent<UIComponent>(fovSlider);

	//	float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - 0.08f));
	//	onSlide(UISliderEvent{ percentage });
	//	}, StandardCursors::Link
	//);

	//AddComponent<UIClickListenerComponent>(fovSlider, [this, slider, fovSlider, onSlide](const MouseButtonReleasedEvent& e) {
	//	auto* sliderComponent = GetComponent<UIComponent>(slider);
	//	auto* backgroundComponent = GetComponent<UIComponent>(fovSlider);

	//	UI::setX(this, slider, sliderComponent, e.x - sliderComponent->GetWidth() / 2);

	//	float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - 0.08f));
	//	onSlide(UISliderEvent{ percentage });
	//});

	//DeactivateEntities(optionsMenu);
}

void StartScene::OnKeyPressed(const KeyPressedEvent& e)
{
	if (e.key == Keys::A)
		Window::setWindowMode(WindowMode::Windowed);
	else if (e.key == Keys::B)
		Window::setWindowMode(WindowMode::WindowedBorderless);
	else if (e.key == Keys::C)
		Window::setWindowMode(WindowMode::WindowedFullscreen);
	else if (e.key == Keys::D)
		Window::setWindowMode(WindowMode::Fullscreen);
}

void StartScene::OnInit()
{
	DeactivateEntities(optionsPanel);
	EventDispatcher::addListener(this, &StartScene::OnKeyPressed);
	EventDispatcher::addListener<MouseMovedEvent>(onMouseMoved);
}
