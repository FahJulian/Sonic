#pragma once
#include "Script.h"

namespace Sonic {

	class DirectionController : public Script
	{
		void OnInit() override
		{
			m_Scene->AddKeyedListener(this, &DirectionController::OnKeyReleased);
		}

		void OnKeyReleased(const KeyReleasedEvent& e)
		{
			auto* component = m_Scene->GetComponent<Direction2DComponent>(m_Entity);
			switch (e.key)
			{
			case Keys::Up: case Keys::W: component->direction = Direction2DComponent::Direction::Up; break;
			case Keys::Down: case Keys::S: component->direction = Direction2DComponent::Direction::Down; break;
			case Keys::Right: case Keys::D: component->direction = Direction2DComponent::Direction::Right; break;
			case Keys::Left: case Keys::A: component->direction = Direction2DComponent::Direction::Left; break;
			}
		}

		void OnDestroy() override
		{
			m_Scene->RemoveKeyedListener<KeyReleasedEvent>(this);
		}
	};

}
