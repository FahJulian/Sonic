#include <vector>
#include <gl/glew.h>
#include "Sonic/Base.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Graphics/Font.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/PairView.h"
#include "FontRenderer.h"

using namespace Sonic;


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

static Shader s_Shader = Shader::Null();
static VertexBuffer s_VBO = VertexBuffer::Null();
static VertexArray s_VAO = VertexArray::Null();

static Vertex s_Vertices[MAX_CHARACTERS * 4];
static std::vector<Font> s_Fonts;

static bool s_CompleteRebuffer = false;


static float indexOf(const Font& font)
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

	void FontRenderer::init()
	{
		s_Shader = Shader(coreResourceDir() + "shaders\\font.vs", coreResourceDir() + "shaders\\font.fs");
		s_Shader.Bind();
		s_Shader.UniformFloat("u_ViewportWidth", Window::getWidth());
		s_Shader.UniformFloat("u_ViewportHeight", Window::getHeight());

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

	void FontRenderer::drawCharacter(int index, float x, float y, float z, unsigned char c, const Font& font, const Color& color)
	{
		float fontIndex = indexOf(font);
		SONIC_LOG_DEBUG_ASSERT(fontIndex != -1, "Error drawing text: Too many fonts");

		Character character = font.GetCharacter(c);

		Vertex* vertex = s_Vertices + 4 * static_cast<size_t>(index);
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

	void FontRenderer::drawString(int index, float x, float y, float z, const String& string, const Font& font, const Color& color)
	{
		size_t size = string.size();
		int kerning = 0;

		for (size_t i = 0; i < size; i++)
		{
			char c = string[i];
			Character ch = font.GetCharacter(c);

			drawCharacter(index++, x + ch.bearingX + kerning, y - (ch.height - ch.bearingY), z, c, font, color);
			x += ch.advanceX + kerning;

			if (i + 1 < size)
				kerning = font.GetKerning(string[i], string[i + 1]);
		}
	}

	void FontRenderer::drawEntity(Scene* scene, Entity e, UITextComponent* t, int index)
	{
		auto* c = scene->GetComponent<UIComponent>(e);

		int textWidth = t->GetFont().StringWidth(t->GetText());
		int textHeight = t->GetFont().StringHeight(t->GetText());

		drawString(index, c->x + (c->width - textWidth) / 2, c->y + (c->height - textHeight) / 2, c->zIndex, t->GetText(), t->GetFont(), t->GetColor());
	}

	void FontRenderer::rebuffer(Scene* scene)
	{
		bool rebuffer = false;

		s_CharacterCount = 0;

		if (s_CompleteRebuffer)
		{
			for (auto [e, t] : scene->View<UITextComponent>())
			{
				int textSize = static_cast<int>(t->GetText().size());
				if (s_CharacterCount + textSize > MAX_CHARACTERS)
					return;

				drawEntity(scene, e, t, s_CharacterCount);
				*t->dirty = false;

				s_CharacterCount += textSize;
			}
		}
		else
		{
			for (auto [e, t] : scene->View<UITextComponent>())
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
		}

		if (rebuffer || s_CompleteRebuffer)
		{
			s_VBO.Bind();
			s_VBO.SetData(s_Vertices, 4 * s_CharacterCount * sizeof(Vertex));
			s_VBO.Unbind();

			s_CompleteRebuffer = false;
		}
	}

	void FontRenderer::render()
	{
		if (s_CharacterCount == 0)
			return;

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

	void FontRenderer::setViewportSize(float width, float height)
	{
		s_Shader.Bind();
		s_Shader.UniformFloat("u_ViewportWidth", width);
		s_Shader.UniformFloat("u_ViewportHeight", height);
		s_Shader.Unbind();

		s_CompleteRebuffer = true;
	}

	void FontRenderer::markDirty()
	{
		s_CompleteRebuffer = true;
	}

	void FontRenderer::destroy()
	{
		s_Fonts.clear();
	}

}
