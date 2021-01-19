#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Sonic {

	struct Character
	{
		const int width, height;
		const int bearingX, bearingY;
		const int advanceX;
		const float x0, x1, y0, y1;
	};

	class Font
	{
	public:
		Font(const std::string& filePath, int size);
		~Font();

		static void init();
		static void destroy();

		int StringWidth(const std::string& string);

		int StringHeight(const std::string& string);

		const Character& GetCharacter(unsigned char c) const;

		int GetKerning(unsigned char c1, unsigned char c2) const;

		bool operator==(const Font& other) { return m_TextureID == other.m_TextureID; }

		FT_Face m_Face;
		unsigned int m_TextureID;
		std::shared_ptr<std::unordered_map<unsigned char, Character>> m_Characters;
	};

}
