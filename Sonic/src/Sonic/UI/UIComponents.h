#pragma once
#include <limits>
#include <string>
#include <vector>
#include "Sonic/Base.h"
#include "Sonic/UI/Font/Font.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Window/Cursor/Cursors.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Entity.h"
#include "UIRenderer.h"
#include "UIEvents.h"
#include "UISize.h"

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
		void SetColor(const Color& color) { properties.color = color; *dirty = true;}
		void SetBorderColor(const Color& color) { properties.borderColor = color; *dirty = true;}
		void SetBorderWeight(float borderWeight) { properties.borderWeight = borderWeight; *dirty = true;}
		void SetEdgeRadius(float edgeRadius) { properties.edgeRadius = edgeRadius; *dirty = true;}

		const Sprite& GetSprite() const { return properties.sprite; }
		const Color& GetColor() const { return properties.color; }
		const Color& GetBorderColor() const { return properties.borderColor; }
		float GetBorderWeight() const { return properties.borderWeight; }
		float GetEdgeRadius() const { return properties.edgeRadius; }

		friend class SceneUIHandler;
		friend class UIRenderer;
	};

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
		void SetBorderWeight(float borderWeight) { properties.borderWeight = borderWeight; *rendererDirty = true; }
		void SetEdgeRadius(float edgeRadius) { properties.edgeRadius = edgeRadius; *rendererDirty = true; }

		const Sprite& GetSprite() const { return properties.sprite; }
		const Color& GetColor() const { return properties.color; }
		const Color& GetBorderColor() const { return properties.borderColor; }
		float GetBorderWeight() const { return properties.borderWeight; }
		float GetEdgeRadius() const { return properties.edgeRadius; }

	private:
		void SetHovered(bool newHovered) { if (newHovered == hovered) *rendererDirty = true; hovered = newHovered; }

		friend class SceneUIHandler;
		friend class UIRenderer;
	};

	struct UIResizableComponent
	{
		struct Borders
		{
			bool bottom;
			bool top;
			bool left;
			bool right;

			operator bool()
			{
				return *reinterpret_cast<uint32_t*>(this) != 0;
			}

			bool operator=(bool b)
			{
				bottom = b;
				top = b;
				left = b;
				right = b;
				return b;
			}
		};

		float grabSize;

		Borders bordersResizable;
		Borders bordersHovered = { false, false, false, false};

		EventListener<UIEntityResizedEvent> onResized;

	public:
		UIResizableComponent(float grabSize = 3.0f, Borders bordersResizable = { true, true, true, true })
			: grabSize(grabSize), bordersResizable(bordersResizable), onResized(nullptr)
		{
		}

		UIResizableComponent(EventListener<UIEntityResizedEvent> onResized, float grabSize = 3.0f, Borders bordersResizable = { true, true, true, true })
			: grabSize(grabSize), bordersResizable(bordersResizable), onResized(onResized)
		{
		}

		friend class SceneUIHandler;
	};

	struct UIMovableComponent
	{
	private:
		StandardCursor cursor;
		EventListener<UIEntityMovedEvent> onMoved;
		bool hovered = false;

	public:
		UIMovableComponent(StandardCursor cursor = StandardCursors::Move)
			: cursor(cursor), onMoved(nullptr)
		{
		}

		UIMovableComponent(EventListener<UIEntityMovedEvent> onMoved, StandardCursor cursor = StandardCursors::Move)
			: cursor(cursor), onMoved(onMoved)
		{
		}

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

	struct UIClickListenerComponent
	{
		bool clicked = false;
		Ref<EventListener<MouseButtonReleasedEvent>> listener;

		UIClickListenerComponent(EventListener<MouseButtonReleasedEvent> listener)
			: listener(createRef<EventListener<MouseButtonReleasedEvent>>(listener)) 
		{
		}
	};

	struct UIComponent
	{
	private:
		using Mode = UISize::Mode;

		Entity parent;

		UISize x;
		UISize y;
		UISize width;
		UISize height;

		float zIndex;

		Ref<bool> uiRendererDirty = Ref<bool>(nullptr);
		Ref<bool> fontRendererDirty = Ref<bool>(nullptr);

	public:
		UIComponent(float x, float y, float width, float height, float zIndex = 0.0f)
			: parent(0), x({ Mode::Absolute, x }), y({ Mode::Absolute, y }), width({ Mode::Absolute, width }), 
			height({ Mode::Absolute, height }), zIndex(zIndex)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, Entity parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), width({ mode, width }), height({ mode, height }), zIndex(0.0f)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, float zIndex, Entity parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), width({ mode, width }), height({ mode, height }), zIndex(zIndex)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, Entity parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(0.0f)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, float zIndex, Entity parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(zIndex)
		{
		}

		float GetX() const { return x.absoluteValue; }
		float GetY() const { return y.absoluteValue; }
		float GetWidth() const { return width.absoluteValue; }
		float GetHeight() const { return height.absoluteValue; }
		float GetZIndex() const { return zIndex; }

		friend class SceneUIHandler;
	};

	struct UIPositionConstraintsComponent
	{
		float minX;
		float minY;
		float maxX;
		float maxY;

		UIPositionConstraintsComponent(float minX, float minY, float maxX, float maxY)
			: minX(minX), minY(minY), maxX(maxX), maxY(maxY)
		{
		}
	};

	struct UISizeConstraintsComponent
	{
		float minWidth;
		float minHeight;
		float maxWidth;
		float maxHeight;

		UISizeConstraintsComponent(float minWidth, float minHeight, float maxWidth, float maxHeight)
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight)
		{
		}
	};

}
