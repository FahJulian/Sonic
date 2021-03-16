#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Sprite.h"

namespace Sonic {

	struct UIRendererProperties
	{
		Sprite sprite;
		Color color;

		Color borderColor = Colors::Black;
		int borderWeight = 0;
		int edgeRadius = 0;

		UIRendererProperties()
			: sprite(Sprite()), color(Colors::Transparent)
		{
		}

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

		UIRendererProperties& Border(Color color, int weight)
		{
			borderColor = color;
			borderWeight = weight;
			return *this;
		}

		UIRendererProperties& EdgeRadius(int radius)
		{
			edgeRadius = radius;
			return *this;
		}

		friend struct UIRendererComponent;
		friend struct UIHoverComponent;
		friend class UIRenderer;
	};

}
