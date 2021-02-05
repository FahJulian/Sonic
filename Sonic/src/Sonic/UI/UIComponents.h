#pragma once
#include <limits>
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

	class Scene;

	struct UIRendererProperties
	{
	private:
		Sprite sprite;
		Color color;

		Color borderColor = Colors::Black;
		float borderWeight = 0.0f;

		float edgeRadius = 0.0f;

	public:
		UIRendererProperties() = delete;

		UIRendererProperties(const Sprite& sprite)
			: sprite(sprite), color(Colors::White)
		{
		}

		UIRendererProperties(const Color& color)
			: sprite(Sprite()), color(color)
		{
		}

		UIRendererProperties(const Sprite& sprite, const Color& color)
			: sprite(sprite), color(color)
		{
		}

		UIRendererProperties& Border(Color color, float weight)
		{
			borderColor = color;
			borderWeight = weight;
			return *this;
		}

		UIRendererProperties& EdgeRadius(float radius)
		{
			edgeRadius = radius;
			return *this;
		}

		friend struct UIRendererComponent;
		friend struct UIHoverComponent;	
		friend class UIRenderer;
	};

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

	private:
		UISize minWidth;
		UISize minHeight;
		UISize maxWidth;
		UISize maxHeight;

		float grabSize;

		Borders bordersResizable;
		Borders bordersHovered = { false, false, false, false};

		bool dirty = true;

	public:
		UIResizableComponent(UISize::Mode mode, float minWidth, float minHeight, float maxWidth, float maxHeight, float grabSize, Borders bordersResizable = { true, true, true, true })
			: minWidth({ mode, minWidth }), minHeight({ mode, minHeight }), maxWidth({ mode, maxWidth }), maxHeight({ mode, maxHeight }), grabSize(grabSize), bordersResizable(bordersResizable)
		{
		}

		UIResizableComponent(UISize minWidth, UISize minHeight, UISize maxWidth, UISize maxHeight, float grabSize, Borders bordersResizable = { true, true, true, true })
			: minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight), grabSize(grabSize), bordersResizable(bordersResizable)
		{
		}

		void SetMinWidth(float newMinWidth) { minWidth.relativeValue = minWidth.relativeValue * (newMinWidth / minWidth.absoluteValue); dirty = true; }
		void SetMinHeight(float newMinHeight) { minHeight.relativeValue = minHeight.relativeValue * (newMinHeight / minHeight.absoluteValue); dirty = true; }
		void SetMaxWidth(float newMaxWidth) { maxWidth.relativeValue = maxWidth.relativeValue * (newMaxWidth / maxWidth.absoluteValue); dirty = true; }
		void SetMaxHeight(float newMaxHeight) { maxHeight.relativeValue = maxHeight.relativeValue * (newMaxHeight / maxHeight.absoluteValue); dirty = true; }

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

	struct UIMovableComponent
	{
	private:
		bool dirty = true;

	public:
		UIMovableComponent() = default;

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
		EventListener<MouseButtonReleasedEvent> listener;

		UIClickListenerComponent(EventListener<MouseButtonReleasedEvent> listener)
			: listener(listener) 
		{
		}
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

		void SetX(float newX) { x.relativeValue = x.relativeValue + (newX - x.absoluteValue) * (width.relativeValue / width.absoluteValue); dirty = true; }
		void SetY(float newY) { y.relativeValue = y.relativeValue + (newY - y.absoluteValue) * (height.relativeValue / height.absoluteValue); dirty = true; }
		void SetWidth(float newWidth) { width.relativeValue = width.relativeValue * (newWidth / width.absoluteValue); dirty = true; }
		void SetHeight(float newHeight) { height.relativeValue = height.relativeValue * (newHeight / height.absoluteValue); dirty = true; }

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

}
