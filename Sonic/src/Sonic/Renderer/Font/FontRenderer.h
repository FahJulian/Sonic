#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Font.h"

namespace Sonic {

	class Scene;

	namespace FontRenderer
	{
		void init();

		void rebuffer(Scene* scene);

		void render();

		void setViewportSize(float width, float height);

		void markDirty();
	}

}
 