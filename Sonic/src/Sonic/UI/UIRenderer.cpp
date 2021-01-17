#include <vector>
#include "Sonic/Base.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Renderer/Buffer/VertexArray.h"
#include "Sonic/Renderer/Buffer/VertexBuffer.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Renderer/Texture.h"
#include "Sonic/Renderer/Shader.h"
#include "UIRenderer.h"

const int MAX_ELEMENTS = 500;
const int MAX_TEXTURES = 16;

struct Vertex
{
	float x, y, zIndex;
	float r, g, b, a;
	float textureX, textureY;
	float textureSlot;

	float rectX, rectY;
	float rectWidth, rectHeight;
	float borderR, borderG, borderB, borderA;
	float borderWeight;
	float edgeRadius;
};


struct Data
{
	Sonic::VertexBuffer vbo;
	Sonic::VertexArray vao;

	int elementCount;
	Vertex vertices[MAX_ELEMENTS * 4];
	Vertex* nextVertex;

	std::vector<Sonic::Texture> textures;
	int textureSlots[16];

	Data()
		: vbo(Sonic::VertexBuffer::Null()), vao(Sonic::VertexArray::Null()),
		elementCount(0), vertices{ 0 }, nextVertex(&vertices[0]) {}

	void Init()
	{
		for (int i = 0; i < MAX_TEXTURES; i++)
			textureSlots[i] = i;

		int* indices = new int[6 * MAX_ELEMENTS];
		for (int i = 0; i < MAX_ELEMENTS; i++)
		{
			indices[6 * i + 0] = 4 * i + 0;
			indices[6 * i + 1] = 4 * i + 1;
			indices[6 * i + 2] = 4 * i + 2;

			indices[6 * i + 3] = 4 * i + 1;
			indices[6 * i + 4] = 4 * i + 3;
			indices[6 * i + 5] = 4 * i + 2;
		}

		vbo = Sonic::VertexBuffer(4 * MAX_ELEMENTS * sizeof(Vertex), { 3, 4, 2, 1, 2, 2, 4, 1, 1 });
		vao = Sonic::VertexArray(indices, MAX_ELEMENTS * 6, { vbo });

		textures.reserve(MAX_TEXTURES);

		delete[] indices;
	}
};


static Data s_Data;
static Sonic::Shader s_Shader = Sonic::Shader::Null();


static float textureSlotOf(const Sonic::Texture& texture)
{
	for (int i = 0; i < s_Data.textures.size(); i++)
	{
		if (s_Data.textures.at(i) == texture)
			return static_cast<float>(i);
	}

	if (s_Data.textures.size() < MAX_TEXTURES)
	{
		s_Data.textures.push_back(texture);
		return static_cast<float>(s_Data.textures.size()) - 1;
	}

	return -1.0f;
}


namespace Sonic {

	namespace UIRenderer {

		void init()
		{
			s_Data.Init();
			s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\ui.vs", SONIC_RESOURCE_DIR + "shaders\\ui.fs");
		}

		void drawElement(float x, float y, float zIndex, float width, float height, const Sprite& sprite, const Color& color, float borderWeight, const Color& borderColor, float edgeRadius)
		{
			if (s_Data.elementCount == MAX_ELEMENTS)
				return;

			float textureSlot = sprite.IsNull() ? -1.0f : textureSlotOf(*sprite.texture);

			for (int i = 0; i < 4; i++)
			{
				s_Data.nextVertex->x = x + (i % 2) * width;
				s_Data.nextVertex->y = y + (i / 2) * height;
				s_Data.nextVertex->zIndex = zIndex;
				s_Data.nextVertex->r = color.r;
				s_Data.nextVertex->g = color.g;
				s_Data.nextVertex->b = color.b;
				s_Data.nextVertex->a = color.a;
				s_Data.nextVertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
				s_Data.nextVertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
				s_Data.nextVertex->textureSlot = textureSlot;
				
				s_Data.nextVertex->rectX = x;
				s_Data.nextVertex->rectY = y;
				s_Data.nextVertex->rectWidth = width;
				s_Data.nextVertex->rectHeight = height;

				s_Data.nextVertex->borderR = borderColor.r;
				s_Data.nextVertex->borderG = borderColor.g;
				s_Data.nextVertex->borderB = borderColor.b;
				s_Data.nextVertex->borderA = borderColor.a;
				s_Data.nextVertex->borderWeight = borderWeight;
				s_Data.nextVertex->edgeRadius = edgeRadius;

				s_Data.nextVertex++;
			}

			s_Data.elementCount++;
		}

		void startScene()
		{
			s_Data.nextVertex = &s_Data.vertices[0];
			s_Data.elementCount = 0;

			s_Shader.Bind();
			s_Shader.UniformFloat("u_WindowWidth", Window::getWidth());
			s_Shader.UniformFloat("u_WindowHeight", Window::getHeight());
			s_Shader.UniformIntArray("u_Textures", s_Data.textureSlots, MAX_TEXTURES);
		}

		void endScene()
		{
			s_Data.vbo.SetData(reinterpret_cast<float*>(s_Data.vertices), 4 * s_Data.elementCount * sizeof(Vertex));

			s_Data.vao.Bind();

			for (int i = 0; i < s_Data.textures.size(); i++)
				s_Data.textures.at(i).Bind(i);

			glDrawElements(GL_TRIANGLES, s_Data.elementCount * 6, GL_UNSIGNED_INT, nullptr);

			for (int i = 0; i < s_Data.textures.size(); i++)
				s_Data.textures.at(i).Unbind();

			s_Shader.Unbind();
			s_Data.vao.Unbind();
		}

	}

}
