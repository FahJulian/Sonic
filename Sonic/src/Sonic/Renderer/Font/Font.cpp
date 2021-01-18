#include <unordered_map>
#include <gl/glew.h>
#include "Sonic/Log/Log.h"
#include "Font.h"

static FT_Library ft;

namespace Sonic {

	void Font::init()
	{
		if (FT_Init_FreeType(&ft) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error initializing FreeType");
	}

	void Font::destroy()
	{
		FT_Done_FreeType(ft);
	}

	Font::Font(const std::string& filePath, int size)
	{
		if (FT_New_Face(ft, filePath.c_str(), 0, &m_Face) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error initializing font face");

		if (FT_Set_Pixel_Sizes(m_Face, 0, size) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error setting char size");

		for (unsigned char c = 0; c < 0xff; c++)
		{
			auto glypIndex = FT_Get_Char_Index(m_Face, c);
			FT_Load_Glyph(m_Face, glypIndex, FT_LOAD_RENDER);
			FT_Render_Glyph(m_Face->glyph, ft_render_mode_normal);
			auto glyph = m_Face->glyph;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				glyph->bitmap.width,
				glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			m_Characters.emplace(c, Character{ texture, static_cast<float>(glyph->bitmap.width), 
				static_cast<float>(glyph->bitmap.rows), static_cast<float>(glyph->bitmap_left), static_cast<float>(glyph->bitmap_top), static_cast<float>(glyph->advance.x >> 6) });
		}
	}

	Font::~Font()
	{
		FT_Done_Face(m_Face);
	}

	const Character& Font::GetCharacter(unsigned char c) const
	{
		return m_Characters.at(c);
	}

	float Font::GetKerning(unsigned char c1, unsigned char c2) const
	{
		auto glyphIndex1 = FT_Get_Char_Index(m_Face, c1);
		auto glyphIndex2 = FT_Get_Char_Index(m_Face, c2);

		FT_Vector kerning;
		if (FT_Get_Kerning(m_Face, glyphIndex1, glyphIndex2, FT_KERNING_DEFAULT, &kerning) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error getting kerning");

		return static_cast<float>(kerning.x >> 6);
	}

}
