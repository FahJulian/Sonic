#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"

namespace Sonic {

	class Scene;

	namespace UIRenderer {

		void init();

		void rebuffer(Scene* scene);

		void render();

		void setViewportSize(float width, float height);

	}

}
