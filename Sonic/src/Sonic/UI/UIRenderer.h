#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"

namespace Sonic {

	class Scene;
	struct UIRendererProperties;

	class UIRenderer
	{
	public:
		static void init();
		static void rebuffer(Scene* scene);
		static void render();
		static void setViewportSize(float width, float height);

	private:
		static void drawElement(int index, float x, float y, float zIndex, float width, float height, const UIRendererProperties* properties);
	};

}
