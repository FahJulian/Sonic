#pragma once
#include <cmath>
#include <string>
#include "Sonic/Graphics/Font/Font.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"

namespace Sonic {

	struct ResizableComponent
	{
		bool edges[4];
		bool dragged[8];
		float grabSize;
		float minWidth;
		float minHeight;
		float maxWidth;
		float maxHeight;

		ResizableComponent(bool bottom, bool right, bool top, bool left, float grabSize, float minWidth, float minHeight, float maxWidth, float maxHeight)
			: edges{ bottom, right, top, left }, dragged{ false }, grabSize(grabSize), minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight) {}
	};

	struct TextComponent
	{
		Font font;
		std::string text;
		Color color;

		TextComponent(Font font, const Color& color, const std::string& text)
			: font(font), text(text), color(color) {}
	};

	struct UIConstraintsComponent
	{
		float x;
		float y;
		float zIndex;
		float width;
		float height;

		UIConstraintsComponent(float x, float y, float width, float height)
			: x(x), y(y), zIndex(0.0f), width(width), height(height) {}
		UIConstraintsComponent(float x, float y, float zIndex, float width, float height)
			: x(x), y(y), zIndex(zIndex), width(width), height(height) {}
		UIConstraintsComponent(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
			: x((float)x), y((float)y), zIndex(0.0f), width((float)width), height((float)height) {}
		UIConstraintsComponent(unsigned int x, unsigned int y, unsigned int zIndex, unsigned int width, unsigned int height)
			: x((float)x), y((float)y), zIndex((float)zIndex), width((float)width), height((float)height) {}
	};

	struct RelativeUIConstraintsComponent
	{
		// 
	};

	struct UIRendererComponent
	{
		Sprite sprite;
		Color color;

		UIRendererComponent(const Sprite& sprite, const Color& color)
			: sprite(sprite), color(color) {}
		UIRendererComponent(const Sprite& sprite)
			: sprite(sprite), color(Colors::White) {}
		UIRendererComponent(const Color& color)
			: sprite(Sprite()), color(color) {}
	};

	struct UIHoverComponent
	{
		bool hovered;
		Sprite sprite;
		Color color;

		UIHoverComponent(const Sprite& sprite, const Color& color)
			: hovered(false), sprite(sprite), color(color) {}
		UIHoverComponent(const Sprite& sprite)
			: hovered(false), sprite(sprite), color(Colors::White) {}
		UIHoverComponent(const Color& color)
			: hovered(false), sprite(Sprite()), color(color) {}
	};

	struct UIBorderComponent
	{
		float weight;
		Color color;

		UIBorderComponent(float weight, const Color& color)
			: weight(weight), color(color) {}
	};

	struct UIRoundedEdgeComponent
	{
		float edgeRadius;

		UIRoundedEdgeComponent(float edgeRadius)
			: edgeRadius(edgeRadius) {}
	};

	struct UIClickListenerComponent
	{
		EventListener<MouseButtonReleasedEvent> listener;

		UIClickListenerComponent(EventListener<MouseButtonReleasedEvent> listener)
			: listener(listener) {}
	};

}
