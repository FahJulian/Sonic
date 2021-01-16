#pragma once
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Entity.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Components.h"

namespace Sonic {

	namespace UI {

		/*Entity createButton(Scene* scene, float x, float y, float width, float height, const Color& color, ClickListenerComponent::ClickListener onClick);

		template<typename F>
		Entity createButton(Scene* scene, float x, float y, float width, float height, F* obj, const Color& color, void(F::* listener)(const MouseButtonReleasedEvent&))
		{
			Entity entity = scene->AddEntity();
			entity.AddComponent<Transform2DComponent>(x, y, width, height);
			entity.AddComponent<ColorComponent>(color);
			entity.AddComponent<ClickListenerComponent>(obj, listener);
			return entity;
		}*/

	}

}
