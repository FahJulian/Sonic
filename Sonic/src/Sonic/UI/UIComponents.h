#pragma once
#include <cmath>
#include <string>
#include "Sonic/Base.h"
#include "Sonic/Log/Log.h"
#include "Sonic/Graphics/Font/Font.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/EntityID.h"
#include "UI.h"
#include "UISize.h"

namespace Sonic {

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
		bool* rendererDirty = nullptr;

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

		void Recalculate(Scene* scene)
		{
			absoluteX = UI::toAbsoluteX(scene, parent, x);
			absoluteY = UI::toAbsoluteY(scene, parent, y);
			absoluteWidth = UI::toAbsoluteWidth(scene, parent, width);
			absoluteHeight = UI::toAbsoluteHeight(scene, parent, height);

			dirty = false;
			if (rendererDirty) 
				*rendererDirty = true;
		}

		void SetX(float newX) { x.value = newX; dirty = true; }
		void SetX(UISize newX) { x = newX; dirty = true; }
		void SetY(float newY) { y.value = newY; dirty = true; }
		void SetY(UISize newY) { y = newY; dirty = true; }
		void SetWidth(float newWidth) { width.value = newWidth; dirty = true; }
		void SetWidth(UISize newWidth) { width = newWidth; dirty = true; }
		void SetHeight(float newHeight) { height.value = newHeight; dirty = true; }
		void SetHeight(UISize newHeight) { height = newHeight; dirty = true; }

		float GetX() const { return absoluteX; }
		float GetY() const { return absoluteY; }
		float GetWidth() const { return absoluteWidth; }
		float GetHeight() const { return absoluteHeight; }
		float GetZIndex() const { return zIndex; }

		bool IsDirty() const { return dirty; }

		friend class Scene;

		friend float UI::toAbsoluteX(Scene* scene, EntityID parent, const UISize& x);
		friend float UI::toAbsoluteY(Scene* scene, EntityID parent, const UISize& y);
		friend float UI::toAbsoluteWidth(Scene* scene, EntityID parent, const UISize& width);
		friend float UI::toAbsoluteHeight(Scene* scene, EntityID parent, const UISize& height);
	};

	struct ResizableComponent
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
		Borders dragged;

		ResizableComponent(UISize::Mode mode, float minWidth, float minHeight, float maxWidth, float maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth({ mode, minWidth }), minHeight({ mode, minHeight }), maxWidth({ mode, maxWidth }), maxHeight({ mode, maxHeight }), grabSize(grabSize), resizable(resizable), dragged({ false, false, false, false})
		{
		}

		ResizableComponent(UISize minWidth, UISize minHeight, UISize maxWidth, UISize maxHeight, float grabSize, Borders resizable = { true, true, true, true })
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight), grabSize(grabSize), resizable(resizable), dragged({ false, false, false, false })
		{
		}
	};

	struct TextComponent
	{
		Font font;
		std::string text;
		Color color;

		TextComponent(Font font, const Color& color, const std::string& text)
			: font(font), text(text), color(color) {}
	};

	struct UIRendererComponent
	{
	//private:
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
	};

	struct UIHoverComponent
	{
		Sprite sprite;
		Color color;

	//private:
		bool hovered;

	public:
		bool* rendererDirty;

		UIHoverComponent(const Sprite& sprite, const Color& color)
			: hovered(false), sprite(sprite), color(color), rendererDirty(nullptr)
		{
		}

		UIHoverComponent(const Sprite& sprite)
			: hovered(false), sprite(sprite), color(Colors::White), rendererDirty(nullptr)
		{
		}

		UIHoverComponent(const Color& color)
			: hovered(false), sprite(Sprite()), color(color), rendererDirty(nullptr)
		{
		}

		void SetHovererd(bool b) { hovered = b; if(rendererDirty) *rendererDirty = true; }
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
