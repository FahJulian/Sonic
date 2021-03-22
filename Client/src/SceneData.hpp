#pragma once

#include "sonic/core/scene/ecs/ComponentPool.h"

#include "sonic/systems/ui/UIRenderer.hpp"

#include "sonic/components/ui/UIComponent.hpp"

#include "c:/dev/Sonic/Sandbox/res/behaviours/TestBehaviour.hpp"

namespace sonic
{
	struct SceneData
	{
		UIRenderer* uiRenderer;

		ComponentPool<UIComponent>* uiComponents;
		ComponentPool<client::TestBehaviour>* client_testBehaviours;
	};
}
