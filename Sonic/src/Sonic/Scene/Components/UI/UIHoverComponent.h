#pragma once

namespace Sonic {

	struct UIHoverComponent
	{
	private:
		UIRendererProperties properties;

		Ref<bool> rendererDirty = createRef<bool>(nullptr);
		bool hovered = false;

	public:
		UIHoverComponent() = delete;

		UIHoverComponent(const UIRendererProperties& properties)
			: properties(properties)
		{
		}

		void SetSprite(const Sprite& sprite) { properties.sprite = sprite; *rendererDirty = true; }
		void SetColor(const Color& color) { properties.color = color; *rendererDirty = true; }
		void SetBorderColor(const Color& color) { properties.borderColor = color; *rendererDirty = true; }
		void SetBorderWeight(int borderWeight) { properties.borderWeight = borderWeight; *rendererDirty = true; }
		void SetEdgeRadius(int edgeRadius) { properties.edgeRadius = edgeRadius; *rendererDirty = true; }

		const Sprite& GetSprite() const { return properties.sprite; }
		const Color& GetColor() const { return properties.color; }
		const Color& GetBorderColor() const { return properties.borderColor; }
		int GetBorderWeight() const { return properties.borderWeight; }
		int GetEdgeRadius() const { return properties.edgeRadius; }

	private:
		void SetHovered(bool newHovered) { if (newHovered == hovered) *rendererDirty = true; hovered = newHovered; }

		friend class SceneUIHandler;
		friend class UIRenderer;
	};

}
