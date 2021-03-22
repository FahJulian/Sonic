#pragma once

#include "sonic/core/scene/Scene.h"

#include "sonic/components/ui/UIComponent.hpp"
#include "sonic/components/ui/UIRendererComponent.hpp"

namespace sonic
{
	class UIRenderer
	{
	public:
		UIRenderer() = default;

		void init(Scene* scene)
		{
			mScene = scene;

			scene->addComponent<UIComponent>(3, UIComponent());
		}

		void update(float deltaSeconds, GroupView<UIComponent, UIRendererComponent>& components)
		{
			for (auto& g : components)
			{

			}
		}

		Scene* mScene;
	};

} // namespace sonic
