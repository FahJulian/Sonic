#pragma once
#include "Sonic/Base.h"
#include "Sonic/Event/Events.h"
#include "Components.h"
#include "Behaviour.h"

namespace Sonic {

	class DirectionController : public Behaviour
	{
		void OnInit() override
		{
			onKeyReleased = AddRemovableListener<KeyReleasedEvent>([this](auto& e) {
				auto* component = GetComponent<DirectionComponent>();
				switch (e.key)
				{
				case Keys::Up: case Keys::W: component->direction = DirectionComponent::Direction::Up; break;
				case Keys::Down: case Keys::S: component->direction = DirectionComponent::Direction::Down; break;
				case Keys::Right: case Keys::D: component->direction = DirectionComponent::Direction::Right; break;
				case Keys::Left: case Keys::A: component->direction = DirectionComponent::Direction::Left; break;
				}
			});
		}

		void OnDestroy() override
		{
			RemoveEventListener<KeyReleasedEvent>(onKeyReleased);
		}

		Ref<int> onKeyReleased;
	};

}
