#pragma once
#include <cmath>
#include <string>
#include <vector>
#include "Sonic/Base.h"
#include "Sonic/Log/Log.h"
#include "Sonic/UI/Font/Font.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/EntityID.h"
#include "UI.h"
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

		float absoluteX = 0;
		float absoluteY = 0;
		float absoluteWidth = 0;
		float absoluteHeight = 0;

		bool dirty = true;
		Ref<bool> uiRendererDirty = Ref<bool>(nullptr);
		Ref<bool> fontRendererDirty = Ref<bool>(nullptr);

		Ref<std::vector<EntityID>> childs = std::make_shared<std::vector<EntityID>>();

	public:
		UIComponent(float x, float y, float width, float height)
			: parent(0), x({ Mode::Absolute, x }), y({ Mode::Absolute, y }), width({ Mode::Absolute, width }), height({ Mode::Absolute, height }), 
			zIndex(0.0f), absoluteX(x), absoluteY(y), absoluteWidth(width), absoluteHeight(height), dirty(false) 
		{
		}

		UIComponent(float x, float y, float zIndex, float width, float height)
			: parent(0), x({ Mode::Absolute, x }), y({ Mode::Absolute, y }), width({ Mode::Absolute, width }), height({ Mode::Absolute, height }),
			zIndex(zIndex), absoluteX(x), absoluteY(y), absoluteWidth(width), absoluteHeight(height), dirty(false) 
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, EntityID parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), zIndex(0.0f), width({ mode, width }), height({ mode, height })
		{
		}

		UIComponent(Mode mode, float x, float y, float zIndex, float width, float height, EntityID parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), zIndex(zIndex), width({ mode, width }), height({ mode, height })
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, EntityID parent = 0)
			: parent(parent), x(x), y(y), zIndex(0.0f), width(width), height(height)
		{
		}

		UIComponent(UISize x, UISize y, float zIndex, UISize width, UISize height, EntityID parent = 0)
			: parent(parent), x(x), y(y), zIndex(zIndex), width(width), height(height)
		{
		}

		void SetX(float newX); 
		void SetY(float newY); 
		void SetWidth(float newWidth) { width.value = (newWidth / absoluteWidth) * width.value; dirty = true; }
		void SetHeight(float newHeight) { height.value = (newHeight / absoluteHeight) * height.value; dirty = true; }

		void SetX(UISize newX) { x = newX; dirty = true; }
		void SetY(UISize newY) { y = newY; dirty = true; }
		void SetWidth(UISize newWidth) { width = newWidth; dirty = true; }
		void SetHeight(UISize newHeight) { height = newHeight; dirty = true; }

		float GetX() const { return absoluteX; }
		float GetY() const { return absoluteY; }
		float GetWidth() const { return absoluteWidth; }
		float GetHeight() const { return absoluteHeight; }
		float GetZIndex() const { return zIndex; }

		bool IsDirty() const { return dirty; }

		void SetRendererDirty()
		{
			if (uiRendererDirty)
				*uiRendererDirty = true;
			if (fontRendererDirty)
				*fontRendererDirty = true;
		}

	private:

		friend class Scene;

		friend float UI::toAbsoluteX(Scene* scene, EntityID parent, const UISize& x);
		friend float UI::toAbsoluteY(Scene* scene, EntityID parent, const UISize& y);
		friend float UI::toAbsoluteWidth(Scene* scene, EntityID parent, const UISize& width);
		friend float UI::toAbsoluteHeight(Scene* scene, EntityID parent, const UISize& height);
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

		UISize minWidth;
		UISize minHeight;
		UISize maxWidth;
		UISize maxHeight;

		float grabSize;

		Borders resizable;
		Borders hovered = { false, false, false, false};
		Borders dragged = { false, false, false, false};

		UIResizableComponent(UISize::Mode mode, float minWidth, float minHeight, float maxWidth, float maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth({ mode, minWidth }), minHeight({ mode, minHeight }), maxWidth({ mode, maxWidth }), maxHeight({ mode, maxHeight }), grabSize(grabSize), resizable(resizable)
		{
		}

		UIResizableComponent(UISize minWidth, UISize minHeight, UISize maxWidth, UISize maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight), grabSize(grabSize), resizable(resizable)
		{
		}
	};

	struct TextComponent
	{
	private:
		Font font;
		Color color;
		std::string text;

	public:
		Ref<bool> dirty;

		TextComponent(Font font, const Color& color, const std::string& text)
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
		void SetHoverered(bool b) { hovered = b; if (uiRendererDirty) *uiRendererDirty = true; }

		const Sprite* GetSprite() const { return &sprite; }
		const Color* GetColor() const { return &color; }
		bool IsHovered() const { return hovered; }
	};

	struct UIBorderComponent
	{
		float weight;
		Color color;

		UIBorderComponent(float weight, const Color& color)
			: weight(weight), color(color) 
		{
		}
	};

	struct UIRoundedEdgeComponent
	{
		float edgeRadius;

		UIRoundedEdgeComponent(float edgeRadius)
			: edgeRadius(edgeRadius) 
		{
		}
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
