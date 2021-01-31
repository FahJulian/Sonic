#include <vector>
#include <gl/glew.h>
#include "Sonic/Base.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Log/Log.h"
#include "Font.h"
#include "FontRenderer.h"

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
	float textureX, textureY;
	float fontIndex;
};

static const int MAX_CHARACTERS = 10000;
static const int MAX_FONTS = 16;

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static int s_CharacterCount;
static Vertex s_Vertices[MAX_CHARACTERS * 4];
static Vertex* s_NextVertex;

static std::vector<Sonic::Font> s_Fonts;
static int s_TextureSlots[MAX_FONTS];

static float indexOf(const Sonic::Font& font)
{
	for (int i = 0; i < s_Fonts.size(); i++)
		if (s_Fonts.at(i) == font)
			return static_cast<float>(i);

	if (s_Fonts.size() < MAX_FONTS)
	{
		s_Fonts.push_back(font);
		return static_cast<float>(s_Fonts.size() - 1);
	}

	return -1.0f;
}


namespace Sonic {

	namespace FontRenderer {

		void init()
		{
			s_CharacterCount = 0;
			s_NextVertex = &s_Vertices[0];

			s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\font.vs", SONIC_RESOURCE_DIR + "shaders\\font.fs");

			int* indices = new int[6 * MAX_CHARACTERS];
			for (int i = 0; i < MAX_CHARACTERS; i++)
			{
				indices[6 * i + 0] = 4 * i + 0;
				indices[6 * i + 1] = 4 * i + 1;
				indices[6 * i + 2] = 4 * i + 2;

				indices[6 * i + 3] = 4 * i + 1;
				indices[6 * i + 4] = 4 * i + 3;
				indices[6 * i + 5] = 4 * i + 2;
			}

			s_VBO = Sonic::VertexBuffer(4 * MAX_CHARACTERS * sizeof(Vertex), { 3, 4, 2, 1 });
			s_VAO = Sonic::VertexArray(indices, MAX_CHARACTERS * 6, { s_VBO });

			s_Fonts.reserve(MAX_FONTS);
			for (int i = 0; i < MAX_FONTS; i++)
				s_TextureSlots[i] = i;

			delete[] indices;
		}

		void startScene()
		{
			s_NextVertex = &s_Vertices[0];
			s_CharacterCount = 0;

			s_Shader.Bind();
			s_Shader.UniformFloat("u_WindowWidth", Window::getWidth());
			s_Shader.UniformFloat("u_WindowHeight", Window::getHeight());
			s_Shader.UniformIntArray("u_Textures", s_TextureSlots, MAX_FONTS);
		}

		void endScene()
		{
			s_VAO.Bind();
			s_Shader.Bind();
			s_VBO.SetData(s_Vertices, 4 * s_CharacterCount * sizeof(Vertex));

			for (int i = 0; i < s_Fonts.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, s_Fonts.at(i).m_TextureID);
			}

			glDrawElements(GL_TRIANGLES, s_CharacterCount * 6, GL_UNSIGNED_INT, nullptr);
		}

		void drawCharacter(float x, float y, float z, unsigned char c, const Font& font, const Color& color)
		{
			if (s_CharacterCount == MAX_CHARACTERS)
				return;

			float fontIndex = indexOf(font);
			if (fontIndex == -1.0f)
				return;

 			Character character = font.GetCharacter(c);

			for (int i = 0; i < 4; i++)
			{
				s_NextVertex->x = x + (i % 2) * character.width;
				s_NextVertex->y = y + (i / 2) * character.height;
				s_NextVertex->z = z;
				s_NextVertex->r = color.r;
				s_NextVertex->g = color.g;
				s_NextVertex->b = color.b;
				s_NextVertex->a = color.a;
				s_NextVertex->textureX = (i % 2 == 0) ? character.x0 : character.x1;
				s_NextVertex->textureY = (i / 2 == 0) ? character.y1 : character.y0;
				s_NextVertex->fontIndex = fontIndex;

				s_NextVertex++;
			}

			s_CharacterCount++;
		}

		void drawString(float x, float y, float z, const std::string& string, const Font& font, const Color& color)
		{
			int size = static_cast<int>(string.size());
			int kerning = 0;
			for (int i = 0; i < size; i++)
			{
				char c = string[i];
				Character ch = font.GetCharacter(c);

				drawCharacter(x + ch.bearingX + kerning, y - (ch.height - ch.bearingY), z, c, font, color);
				x += ch.advanceX + kerning;

				if (i + 1 < size)
					kerning = font.GetKerning(string[i], string[i + 1]);
			}
		}

	}

}
