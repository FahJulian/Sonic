#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Sonic/Base.h"

struct FT_FaceRec_;

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
		Font(const String& filePath, int size);
		~Font();

		static void init();
		static void destroy();

		void Bind(int slot) const;
		void Unbind() const;

		int StringWidth(const String& string) const;
		int StringHeight(const String& string) const;

		const Character& GetCharacter(unsigned char c) const;
		int GetKerning(unsigned char c1, unsigned char c2) const;

		bool operator==(const Font& other) const { return m_TextureID == other.m_TextureID; }

		FT_FaceRec_* m_Face;
		Ref<unsigned int> m_TextureID;
		Ref<std::unordered_map<unsigned char, Character>> m_Characters;
		Ref<std::unordered_map<unsigned short, int>> m_KnownKerning;
	};

}
