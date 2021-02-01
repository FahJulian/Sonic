#pragma once
#include "Sonic/Graphics/Color.h"
#include "Font.h"

namespace Sonic {

	class Scene;

	namespace FontRenderer
	{
		void init();

		void drawCharacter(float x, float y, float z, unsigned char c, const Font& font, const Color& color);

		void drawString(float x, float y, float z, const std::string& string, const Font& font, const Color& color);

		void update(Scene* scene);

		void render();
	}

}
 