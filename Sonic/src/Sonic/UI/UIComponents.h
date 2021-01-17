#pragma once
#include "Sonic/Renderer/Sprite.h"
#include "Sonic/Renderer/Color.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events.h"

namespace Sonic {

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
		UIConstraintsComponent(int x, int y, int width, int height)
			: x((float)x), y((float)y), zIndex(0.0f), width((float)width), height((float)height) {}
		UIConstraintsComponent(int x, int y, int zIndex, int width, int height)
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
