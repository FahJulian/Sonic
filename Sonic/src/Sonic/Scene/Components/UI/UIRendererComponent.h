#pragma once
#include "Sonic/Renderer/UI/UIRendererProperties.h"

namespace Sonic {

	class Scene;

	struct UIRendererComponent
	{
	private:
		UIRendererProperties properties;
		Ref<bool> dirty = createRef<bool>(true);

	public:
		UIRendererComponent() = delete;

		UIRendererComponent(const UIRendererProperties& properties)
			: properties(properties)
		{
		}

		void SetSprite(const Sprite& sprite) { properties.sprite = sprite; *dirty = true; }
		void SetColor(const Color& color) { properties.color = color; *dirty = true; }
		void SetBorderColor(const Color& color) { properties.borderColor = color; *dirty = true; }
		void SetBorderWeight(float borderWeight) { properties.borderWeight = borderWeight; *dirty = true; }
		void SetEdgeRadius(float edgeRadius) { properties.edgeRadius = edgeRadius; *dirty = true; }

		const Sprite& GetSprite() const { return properties.sprite; }
		const Color& GetColor() const { return properties.color; }
		const Color& GetBorderColor() const { return properties.borderColor; }
		float GetBorderWeight() const { return properties.borderWeight; }
		float GetEdgeRadius() const { return properties.edgeRadius; }

		friend class SceneUIHandler;
		friend class UIRenderer;
	};

}
