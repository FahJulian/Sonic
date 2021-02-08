#pragma once

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

	class UIRenderer
	{
	public:
		static void init();
		static void rebuffer(Scene* scene);
		static void render();
		static void setViewportSize(float width, float height);

	private:
		static void drawElement(int index, float x, float y, float width, float height, float zIndex, const UIRendererProperties* properties);
	};

}
