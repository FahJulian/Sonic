#include <vector>
#include "Sonic/Base.h"
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Graphics2D/Texture.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "UIRenderer.h"
#include "UIComponents.h"

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

static const int MAX_ELEMENTS = 500;
static const int MAX_TEXTURES = 16;

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static int s_ElementCount;
static Vertex s_Vertices[MAX_ELEMENTS * 4];
static Vertex* s_NextVertex;

static std::vector<Sonic::Texture> s_Textures;
static int s_TextureSlots[MAX_TEXTURES];

static float textureSlotOf(const Sonic::Texture& texture)
{
	for (int i = 0; i < s_Textures.size(); i++)
	{
		if (s_Textures.at(i) == texture)
			return static_cast<float>(i);
	}

	if (s_Textures.size() < MAX_TEXTURES)
	{
		s_Textures.push_back(texture);
		return static_cast<float>(s_Textures.size()) - 1;
	}

	return -1.0f;
}


namespace Sonic {

	namespace UIRenderer {

		void init()
		{
			s_ElementCount = 0;
			s_NextVertex = &s_Vertices[0];

			s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\ui.vs", SONIC_RESOURCE_DIR + "shaders\\ui.fs");

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

			s_VBO = Sonic::VertexBuffer(4 * MAX_ELEMENTS * sizeof(Vertex), { 3, 4, 2, 1, 2, 2, 4, 1, 1 });
			s_VAO = Sonic::VertexArray(indices, MAX_ELEMENTS * 6, { s_VBO });

			delete[] indices;

			s_Textures.reserve(MAX_TEXTURES);
			for (int i = 0; i < MAX_TEXTURES; i++)
				s_TextureSlots[i] = i;
		}

		void drawElement(float x, float y, float zIndex, float width, float height, const Sprite& sprite, const Color& color, float borderWeight, const Color& borderColor, float edgeRadius)
		{
			SONIC_PROFILE_FUNCTION("UIRenderer::drawElement");

			if (s_ElementCount == MAX_ELEMENTS)
				return;

			float textureSlot = sprite.IsNull() ? -1.0f : textureSlotOf(*sprite.texture);

			for (int i = 0; i < 4; i++)
			{
				s_NextVertex->x = x + (i % 2) * width;
				s_NextVertex->y = y + (i / 2) * height;
				s_NextVertex->zIndex = zIndex;
				s_NextVertex->r = color.r;
				s_NextVertex->g = color.g;
				s_NextVertex->b = color.b;
				s_NextVertex->a = color.a;
				s_NextVertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
				s_NextVertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
				s_NextVertex->textureSlot = textureSlot;
				
				s_NextVertex->rectX = x;
				s_NextVertex->rectY = y;
				s_NextVertex->rectWidth = width;
				s_NextVertex->rectHeight = height;

				s_NextVertex->borderR = borderColor.r;
				s_NextVertex->borderG = borderColor.g;
				s_NextVertex->borderB = borderColor.b;
				s_NextVertex->borderA = borderColor.a;
				s_NextVertex->borderWeight = borderWeight;
				s_NextVertex->edgeRadius = edgeRadius;

				s_NextVertex++;
			}

			s_ElementCount++;
		}

		void startScene()
		{
			SONIC_PROFILE_FUNCTION("UIRenderer::startScene");

			s_NextVertex = &s_Vertices[0];
			s_ElementCount = 0;

			s_Shader.Bind();
			s_Shader.UniformFloat("u_WindowWidth", Window::getWidth());
			s_Shader.UniformFloat("u_WindowHeight", Window::getHeight());
			s_Shader.UniformIntArray("u_Textures", s_TextureSlots, MAX_TEXTURES);
		}

		void endScene()
		{
			SONIC_PROFILE_FUNCTION("UIRenderer::endScene");

			s_Shader.Bind();
			s_VBO.SetData(reinterpret_cast<float*>(s_Vertices), 4 * s_ElementCount * sizeof(Vertex));

			s_VAO.Bind();
			s_Shader.Bind();

			for (int i = 0; i < s_Textures.size(); i++)
				s_Textures.at(i).Bind(i);

			glDrawElements(GL_TRIANGLES, s_ElementCount * 6, GL_UNSIGNED_INT, nullptr);

			for (int i = 0; i < s_Textures.size(); i++)
				s_Textures.at(i).Unbind();

			s_Shader.Unbind();
			s_VAO.Unbind();
		}

	}

}
