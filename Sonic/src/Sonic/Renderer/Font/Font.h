#pragma once
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Sonic {

	struct Character
	{
		const unsigned int textureID;
		const float width, height;
		const float bearingX, bearingY;
		const float advanceX;
	};

	class Font
	{
	public:
		Font(const std::string& filePath, int size);
		~Font();

		static void init();
		static void destroy();

		const Character& GetCharacter(unsigned char c) const;

		float GetKerning(unsigned char c1, unsigned char c2) const;

		FT_Face m_Face;
		std::unordered_map<unsigned char, Character> m_Characters;
	};

}
