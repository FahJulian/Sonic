#include <unordered_map>
#include <vector>
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
		: m_TextureID(std::make_shared<unsigned int>()),
		m_Characters(std::make_shared<std::unordered_map<unsigned char, Character>>()),
		m_KnownKerning(std::make_shared<std::unordered_map<unsigned short, int>>())
	{
		if (FT_New_Face(ft, filePath.c_str(), 0, &m_Face) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error initializing font face");

		if (FT_Set_Pixel_Sizes(m_Face, 0, size) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error setting char size");

		std::vector<int> widths;
		std::vector<int> heights;
		widths.reserve(0xff);
		heights.reserve(0xff);

		for (unsigned char c = 0; c < 0xff; c++)
		{
			auto glypIndex = FT_Get_Char_Index(m_Face, c);
			FT_Load_Glyph(m_Face, glypIndex, FT_LOAD_RENDER);
			FT_Render_Glyph(m_Face->glyph, ft_render_mode_normal);

			widths.push_back(m_Face->glyph->bitmap.width + 1);
			heights.push_back(m_Face->glyph->bitmap.rows);
		}

		int maxHeight = 0;
		for (int i : heights)
			if (i > maxHeight)
				maxHeight = i;

		int totalWidth = 0;
		for (int i : widths)
			totalWidth += i;

		unsigned char* giantBitmap = new unsigned char[maxHeight * totalWidth]{ 0 };

		unsigned int currentX = 0;
		for (unsigned char c = 0; c < 0xff; c++)
		{
			auto glypIndex = FT_Get_Char_Index(m_Face, c);
			FT_Load_Glyph(m_Face, glypIndex, FT_LOAD_RENDER);
			FT_Render_Glyph(m_Face->glyph, ft_render_mode_normal);
			auto glyph = m_Face->glyph;

			int width = static_cast<int>(glyph->bitmap.width);
			int height = static_cast<int>(glyph->bitmap.rows);
			int bearingX = static_cast<int>(glyph->bitmap_left);
			int bearingY = static_cast<int>(glyph->bitmap_top);
			int advanceX = static_cast<int>(glyph->advance.x >> 6);

			for (int y = 0; y < height; y++)
				for (int x = 0; x < width; x++)
					giantBitmap[currentX + x + y * totalWidth] = glyph->bitmap.buffer[x + y * width];

			float x0 = static_cast<float>(currentX) / totalWidth;
			float x1 = static_cast<float>(currentX + width) / totalWidth;
			float y0 = 0.0f;
			float y1 = static_cast<float>(height + 1) / maxHeight;

			m_Characters->emplace(c, Character{ width, height, 
				bearingX, bearingY, advanceX, x0, x1, y0, y1 });

			currentX += glyph->bitmap.width;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, m_TextureID.get());
		glBindTexture(GL_TEXTURE_2D, *m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, totalWidth, maxHeight, 0, GL_RED, GL_UNSIGNED_BYTE, giantBitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	Font::~Font()
	{
		if (m_Characters.use_count() == 1)
			FT_Done_Face(m_Face);
		if (m_TextureID.use_count() == 1)
			glDeleteTextures(1, m_TextureID.get());
	}

	void Font::Bind(int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, *m_TextureID);
	}

	void Font::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const Character& Font::GetCharacter(unsigned char c) const
	{
		return m_Characters->at(c);
	}

	int Font::GetKerning(unsigned char c1, unsigned char c2) const
	{
		unsigned short index = c1 > c2 ? c1 << 8 | c2 : c2 << 8 | c1;

		auto known = m_KnownKerning->find(index);
		if (known != m_KnownKerning->end())
			return known->second;

		auto glyphIndex1 = FT_Get_Char_Index(m_Face, c1);
		auto glyphIndex2 = FT_Get_Char_Index(m_Face, c2);

		FT_Vector kerning;
		if (FT_Get_Kerning(m_Face, glyphIndex1, glyphIndex2, FT_KERNING_DEFAULT, &kerning) != FT_Err_Ok)
			SONIC_LOG_DEBUG("Error getting kerning");

		int k = static_cast<int>(kerning.x >> 6);
		m_KnownKerning->emplace(index, k);
		return k;
	}

	int Font::StringWidth(const std::string& string) const
	{
		int width = 0;

		int size = static_cast<int>(string.size());
		int kerning = 0;

		for (int i = 0; i < size; i++)
		{
			width += GetCharacter(string[i]).advanceX + kerning;
			if (i + 1 < size)
				kerning = GetKerning(string[i], string[i + 1]);
		}

		return width;
	}

	int Font::StringHeight(const std::string& string) const
	{
		int maxHeight = 0;

		for (char c : string)
		{
			int height = GetCharacter(c).height;
			if (height > maxHeight)
				maxHeight = height;
		}

		return maxHeight;
	}

}
