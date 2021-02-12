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
		static void drawElement(int index, float x, float y, float width, float height, float zIndex, const UIRendererProperties* properties);
	};

}
