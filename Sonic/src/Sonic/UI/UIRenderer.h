#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Graphics2D/Sprite.h"

namespace Sonic {

	namespace UIRenderer {

		void init();

		void drawElement(float x, float y, float zIndex, float width, float height, const Sprite& sprite, const Color& color, float borderWeight, const Color& borderColor, float edgeRadius);

		void startScene();
		void endScene();
	}

}
