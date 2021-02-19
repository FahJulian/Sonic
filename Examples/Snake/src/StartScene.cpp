#include "StartScene.h"
#include "Settings.h"

#include <functional>
#include "Sonic/Scene/UI/UIUtils.h"

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

void StartScene::OnLoad()
{
	Font font  = Font(coreResourceDir() + "fonts/calibri.ttf", static_cast<int>(0.055f * Window::getHeight()));

	EntityGroup startMenu = AddEntityGroup();
	EntityGroup optionsMenu = AddEntityGroup();
	Entity button;

	Animation* animation = new Animation(startMenu, 0.2f, 50.0f);
	AddComponent<ScriptComponent>(startMenu, animation);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 240, 420, 40, UIRendererProperties(Colors::LightGray).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [=](const auto& e) {}, font, Colors::Red, "Singleplayer");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 190, 420, 40, UIRendererProperties(Colors::Red).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [=](const auto& e) { animation->Start(); }, font, 0xddddddff, "Multiplayer");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 140, 420, 40, UIRendererProperties(Colors::DarkBlue).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {}, font, 0xddddddff, "Minecraft Realms");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 150, 70, 40, 40, UIRendererProperties(Colors::Transparent).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) {});
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 210, 70, 200, 40, UIRendererProperties(Colors::Green).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [=](const auto& e) { DeactivateEntities(startMenu); ReactivateEntities(optionsMenu); }, font, 0xddddddff, "Options...");
	AddToGroup(startMenu, button);

	button = UI::createButton(this, UISize::Mode::Absolute, 430, 70, 200, 40, UIRendererProperties(Colors::Orange).Border(Colors::Black, 2),
		UIRendererProperties(Colors::Cyan).Border(Colors::Black, 2), [](const auto& e) { App::stop(); }, font, 0xddddddff, "Quit Game");
	AddToGroup(startMenu, button);

	Entity optionsText = AddEntity(optionsMenu);
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

	Entity fovSlider = AddEntity(optionsMenu);
	AddComponent<UIComponent>(fovSlider,
		UISize{ UISize::Mode::WindowCenter, 0.35f },
		UISize{ UISize::Mode::RelativeToWindow, 0.75f },
		UISize{ UISize::Mode::RelativeToWindow, 0.3f },
		UISize{ UISize::Mode::Absolute, 40 }
	);
	AddComponent<UIRendererComponent>(fovSlider, UIRendererProperties(Colors::DarkGray).Border(Colors::Black, 2));

	Entity slider = AddEntity(optionsMenu);
	AddComponent<UIComponent>(slider, UISize::Mode::RelativeToEntity, (1.0f - 0.08f) / 2.0f, 0.0f, 0.08f, 1.0f, fovSlider);
	AddComponent<UIRendererComponent>(slider, UIRendererProperties(Colors::LightGray).Border(Colors::Black, 2));
	AddComponent<UIPositionConstraintsComponent>(slider, 0.0f, 0.0f, 1.0f - 0.08f, 0.0f);

	EventListener<UISliderEvent> onSlide = [=](const UISliderEvent& e) {
		SONIC_LOG_DEBUG("Fov: ", (int)(80 * e.percentage + 30));
	};

	AddComponent<UIMovableComponent>(slider, [this, slider, fovSlider, onSlide](const UIEntityMovedEvent& e) {
		auto* sliderComponent = GetComponent<UIComponent>(slider);
		auto* backgroundComponent = GetComponent<UIComponent>(fovSlider);

		float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - 0.08f));
		onSlide(UISliderEvent{ percentage });
		}, StandardCursors::Link
	);

	AddComponent<UIClickListenerComponent>(fovSlider, [this, slider, fovSlider, onSlide](const MouseButtonReleasedEvent& e) {
		auto* sliderComponent = GetComponent<UIComponent>(slider);
		auto* backgroundComponent = GetComponent<UIComponent>(fovSlider);

		UI::setX(this, slider, sliderComponent, e.x - sliderComponent->GetWidth() / 2);

		float percentage = (sliderComponent->GetX() - backgroundComponent->GetX()) / (backgroundComponent->GetWidth() * (1.0f - 0.08f));
		onSlide(UISliderEvent{ percentage });
	});

	DeactivateEntities(optionsMenu);
}

void StartScene::OnKeyPressed(const KeyPressedEvent& e)
{
	if (e.key == Keys::A)
		Window::setWindowMode(WindowMode::Windowed);
	else if (e.key == Keys::B)
		Window::setWindowMode(WindowMode::Borderless);
	else if (e.key == Keys::C)
		Window::setWindowMode(WindowMode::BorderlessFullscreen);
	else if (e.key == Keys::D)
		Window::setWindowMode(WindowMode::Fullscreen);
}

void StartScene::OnInit()
{
	EventDispatcher::addListener(this, &StartScene::OnKeyPressed);
}
