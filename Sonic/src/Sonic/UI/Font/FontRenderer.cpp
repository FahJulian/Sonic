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
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/PairView.h"

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
	float textureX, textureY;
	float fontIndex;
};

static const int MAX_CHARACTERS = 10000;
static const int MAX_FONTS = 16;

static int s_CharacterCount;

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static Vertex s_Vertices[MAX_CHARACTERS * 4];
static std::vector<Sonic::Font> s_Fonts;

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
			s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\font.vs", SONIC_RESOURCE_DIR + "shaders\\font.fs");
			s_Shader.Bind();
			s_Shader.UniformFloat("u_WindowWidth", Window::getWidth());
			s_Shader.UniformFloat("u_WindowHeight", Window::getHeight());

			int textureSlots[MAX_FONTS];
			for (int i = 0; i < MAX_FONTS; i++)
				textureSlots[i] = i;

			s_Shader.UniformIntArray("u_Textures", textureSlots, MAX_FONTS);
			s_Shader.Unbind();

			s_Fonts.reserve(MAX_FONTS);

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

			delete[] indices;
		}

		void drawCharacter(int index, float x, float y, float z, unsigned char c, const Font& font, const Color& color)
		{
			float fontIndex = indexOf(font);
			SONIC_LOG_DEBUG_ASSERT(fontIndex != -1, "Error drawing text: Too many fonts");

			Character character = font.GetCharacter(c);

			Vertex* vertex = s_Vertices + 4 * index;
			for (int i = 0; i < 4; i++)
			{
				vertex->x = x + (i % 2) * character.width;
				vertex->y = y + (i / 2) * character.height;
				vertex->z = z;
				vertex->r = color.r;
				vertex->g = color.g;
				vertex->b = color.b;
				vertex->a = color.a;
				vertex->textureX = (i % 2 == 0) ? character.x0 : character.x1;
				vertex->textureY = (i / 2 == 0) ? character.y1 : character.y0;
				vertex->fontIndex = fontIndex;

				vertex++;
			}
		}

		void drawString(int index, float x, float y, float z, const std::string& string, const Font& font, const Color& color)
		{
			int size = static_cast<int>(string.size());
			int kerning = 0;

			for (int i = 0; i < size; i++)
			{
				char c = string[i];
				Character ch = font.GetCharacter(c);

				drawCharacter(index++, x + ch.bearingX + kerning, y - (ch.height - ch.bearingY), z, c, font, color);
				x += ch.advanceX + kerning;

				if (i + 1 < size)
					kerning = font.GetKerning(string[i], string[i + 1]);
			}
		}

		void drawEntity(Scene* scene, EntityID e, TextComponent* t, int index)
		{
			auto* c = scene->GetComponent<UIComponent>(e);

			int textWidth = t->GetFont().StringWidth(t->GetText());
			int textHeight = t->GetFont().StringHeight(t->GetText());

			drawString(index, c->GetX() + (c->GetWidth() - textWidth) / 2, c->GetY() + (c->GetHeight() - textHeight) / 2, c->GetZIndex(), t->GetText(), t->GetFont(), t->GetColor());
		}

		void update(Scene* scene)
		{
			bool rebuffer = false;

			s_CharacterCount = 0;
			for (auto [e, t] : scene->View<TextComponent>())
			{
				int textSize = static_cast<int>(t->GetText().size());
				if (s_CharacterCount + textSize > MAX_CHARACTERS)
					return;

				if (*t->dirty)
				{
					drawEntity(scene, e, t, s_CharacterCount);
					*t->dirty = false;
					rebuffer = true;
				}

				s_CharacterCount += textSize;
			}

			if (rebuffer)
			{
				s_VBO.Bind();
				s_VBO.SetData(s_Vertices, 4 * s_CharacterCount * sizeof(Vertex));
				s_VBO.Unbind();
			}
		}

		void render()
		{
			s_Shader.Bind();
			s_VAO.Bind();

			for (int i = 0; i < s_Fonts.size(); i++)
				s_Fonts.at(i).Bind(i);

			glDrawElements(GL_TRIANGLES, s_CharacterCount * 6, GL_UNSIGNED_INT, nullptr);

			for (int i = 0; i < s_Fonts.size(); i++)
				s_Fonts.at(i).Unbind();

			s_VAO.Unbind();
			s_Shader.Unbind();
		}

	}

}
