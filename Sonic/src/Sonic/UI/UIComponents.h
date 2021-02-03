#pragma once
#include <string>
#include <vector>
#include "Sonic/Base.h"
#include "Sonic/UI/Font/Font.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/EntityID.h"
#include "UISize.h"

namespace Sonic {

	struct UIRendererComponent
	{
	private:
		Sprite sprite;
		Color color;

	public:
		Ref<bool> dirty;

		UIRendererComponent(const Sprite& sprite, const Color& color)
			: sprite(sprite), color(color), dirty(new bool(true))
		{
		}

		UIRendererComponent(const Sprite& sprite)
			: sprite(sprite), color(Colors::White), dirty(new bool(true))
		{
		}

		UIRendererComponent(const Color& color)
			: sprite(Sprite()), color(color), dirty(new bool(true))
		{
		}

		void SetSprite(const Sprite& newSprite) { sprite = newSprite; *dirty = true; }
		void SetColor(const Color& newColor) { color = newColor; *dirty = true; }

		const Sprite* GetSprite() const { return &sprite; }
		const Color* GetColor() const { return &color; }
	};

	struct UIComponent
	{
	private:
		using Mode = UISize::Mode;

		EntityID parent;
		UISize x;
		UISize y;
		UISize width;
		UISize height;

		float zIndex;

		bool dirty = true;

		Ref<bool> uiRendererDirty = Ref<bool>(nullptr);
		Ref<bool> fontRendererDirty = Ref<bool>(nullptr);

		Ref<std::vector<EntityID>> childs = std::make_shared<std::vector<EntityID>>();

	public:
		UIComponent(float x, float y, float width, float height, float zIndex = 0.0f)
			: parent(0), x({ Mode::Absolute, x, x }), y({ Mode::Absolute, y, y }), width({ Mode::Absolute, width, width }), height({ Mode::Absolute, height, height }),
			zIndex(zIndex), dirty(false) 
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, EntityID parent = 0)
			: parent(parent), x({ mode, x, 0.0f }), y({ mode, y, 0.0f }), width({ mode, width, 0.0f }), height({ mode, height, 0.0f }), zIndex(0.0f)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, float zIndex, EntityID parent = 0)
			: parent(parent), x({ mode, x, 0.0f }), y({ mode, y, 0.0f }), width({ mode, width, 0.0f }), height({ mode, height, 0.0f }), zIndex(zIndex)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, EntityID parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(0.0f)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, float zIndex, EntityID parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(zIndex)
		{
		}

		void SetX(float newX) { x.value = x.value + (x.absoluteValue - newX) * (width.value / width.absoluteValue); dirty = true; }
		void SetY(float newY) { y.value = y.value + (y.absoluteValue - newY) * (height.value / height.absoluteValue); dirty = true; }
		void SetWidth(float newWidth) { width.value = width.value * (newWidth / width.absoluteValue); dirty = true; }
		void SetHeight(float newHeight) { height.value = height.value * (newHeight / height.absoluteValue); dirty = true; }

		void SetX(UISize newX) { x = newX; dirty = true; }
		void SetY(UISize newY) { y = newY; dirty = true; }
		void SetWidth(UISize newWidth) { width = newWidth; dirty = true; }
		void SetHeight(UISize newHeight) { height = newHeight; dirty = true; }

		float GetX() const { return x.absoluteValue; }
		float GetY() const { return y.absoluteValue; }
		float GetWidth() const { return width.absoluteValue; }
		float GetHeight() const { return height.absoluteValue; }
		float GetZIndex() const { return zIndex; }

		bool IsDirty() const { return dirty; }

		void SetRendererDirty()
		{
			if (uiRendererDirty)
				*uiRendererDirty = true;
			if (fontRendererDirty)
				*fontRendererDirty = true;
		}

		friend class SceneUIHandler;
	};

	struct UIResizableComponent
	{
		struct Borders
		{
			bool bottom;
			bool top;
			bool left;
			bool right;
		};

	private:
		UISize minWidth;
		UISize minHeight;
		UISize maxWidth;
		UISize maxHeight;

		float grabSize;

		Borders resizable;
		Borders hovered = { false, false, false, false};
		Borders dragged = { false, false, false, false };

		bool dirty = true;

	public:
		UIResizableComponent(UISize::Mode mode, float minWidth, float minHeight, float maxWidth, float maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth({ mode, minWidth }), minHeight({ mode, minHeight }), maxWidth({ mode, maxWidth }), maxHeight({ mode, maxHeight }), grabSize(grabSize), resizable(resizable)
		{
		}

		UIResizableComponent(UISize minWidth, UISize minHeight, UISize maxWidth, UISize maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight), grabSize(grabSize), resizable(resizable)
		{
		}

		void SetMinWidth(float newMinWidth) { minWidth.value = minWidth.value * (newMinWidth / minWidth.absoluteValue); dirty = true; }
		void SetMinHeight(float newMinHeight) { minHeight.value = minHeight.value * (newMinHeight / minHeight.absoluteValue); dirty = true; }
		void SetMaxWidth(float newMaxWidth) { maxWidth.value = maxWidth.value * (newMaxWidth / maxWidth.absoluteValue); dirty = true; }
		void SetMaxHeight(float newMaxHeight) { maxHeight.value = maxHeight.value * (newMaxHeight / maxHeight.absoluteValue); dirty = true; }

		void SetMinWidth(UISize newMinWidth) { minWidth = newMinWidth; dirty = true; }
		void SetMinHeight(UISize newMinHeight) { minHeight= newMinHeight; dirty = true; }
		void SetMaxWidth(UISize newMaxWidth) { maxWidth = newMaxWidth; dirty = true; }
		void SetMaxHeight(UISize newMaxHeight) { maxHeight = newMaxHeight; dirty = true; }

		float GetMinWidth() const { return minWidth.absoluteValue; }
		float GetMaxWidth() const { return maxWidth.absoluteValue; }
		float GetMinHeight() const { return minHeight.absoluteValue; }
		float GetMaxHeight() const { return maxHeight.absoluteValue; }

		bool IsDirty() const { return dirty; }

		friend class SceneUIHandler;
	};

	struct UITextComponent
	{
	private:
		Font font;
		Color color;
		std::string text;

	public:
		Ref<bool> dirty;

		UITextComponent(Font font, const Color& color, const std::string& text)
			: font(font), color(color), text(text), dirty(new bool(true))
		{
		}

		const Font& GetFont() const { return font; }
		const Color& GetColor() const { return color; }
		const std::string& GetText() const { return text; }

		void SetFont(const Font& newFont) { font = newFont; *dirty = true; }
		void SetColor(const Color& newColor) { color = newColor; *dirty = true; }
		void SetText(const std::string& newText) { text = newText; *dirty = true; }
	};

	struct UIHoverComponent
	{
	private:
		Sprite sprite;
		Color color;

		bool hovered;

	public:
		Ref<bool> uiRendererDirty;

		UIHoverComponent(const Sprite& sprite, const Color& color)
			: hovered(false), sprite(sprite), color(color), uiRendererDirty(nullptr)
		{
		}

		UIHoverComponent(const Sprite& sprite)
			: hovered(false), sprite(sprite), color(Colors::White), uiRendererDirty(nullptr)
		{
		}

		UIHoverComponent(const Color& color)
			: hovered(false), sprite(Sprite()), color(color), uiRendererDirty(nullptr)
		{
		}

		void SetSprite(const Sprite& newSprite) { sprite = newSprite; if (uiRendererDirty && hovered) *uiRendererDirty = true; }
		void SetColor(const Color& newColor) { color = newColor; if (uiRendererDirty && hovered) *uiRendererDirty = true; }
		void SetHoverered(bool b) { if (b == hovered) return; hovered = b; if (uiRendererDirty) *uiRendererDirty = true; }

		const Sprite* GetSprite() const { return &sprite; }
		const Color* GetColor() const { return &color; }
		bool IsHovered() const { return hovered; }
	};

	struct UIBorderComponent
	{
	private:
		float weight;
		Color color;

	public:
		UIBorderComponent(float weight, const Color& color)
			: weight(weight), color(color) 
		{
		}

		float GetWeight() const { return weight; }
		const Color& GetColor() const { return color; }
	};

	struct UIRoundedEdgeComponent
	{
	private:
		float edgeRadius;

	public:
		UIRoundedEdgeComponent(float edgeRadius)
			: edgeRadius(edgeRadius) 
		{
		}

		float GetEdgeRadius() const { return edgeRadius; }
	};

	struct UIClickListenerComponent
	{
		EventListener<MouseButtonReleasedEvent> listener;

		UIClickListenerComponent(EventListener<MouseButtonReleasedEvent> listener)
			: listener(listener) 
		{
		}
	};

}
