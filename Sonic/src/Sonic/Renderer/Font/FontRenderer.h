#pragma once
#include "Font.h"

namespace Sonic {

	namespace FontRenderer
	{
		void init();

		void drawCharacter(float x, float y, unsigned char c, const Font& font);

		void drawString(float x, float y, const std::string& string, const Font& font);

		void startScene();

		void endScene();
	}

}
