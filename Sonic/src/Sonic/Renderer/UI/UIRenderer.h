#pragma once
#include "UIRendererProperties.h"

namespace Sonic {

	class Scene;

	class UIRenderer
	{
	public:
		static void init();
		static void rebuffer(Scene* scene);
		static void render();
		static void setViewportSize(float width, float height);
		static void markDirty();
		static void destroy();

	private:
		static void drawElement(int index, int x, int y, int width, int height, float zIndex, const UIRendererProperties* properties);
	};

}
