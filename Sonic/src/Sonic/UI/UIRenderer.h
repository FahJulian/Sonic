#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"

namespace Sonic {

	class Scene;

	namespace UIRenderer {

		void init();
		void update(Scene* scene);
		void render();

	}

}
