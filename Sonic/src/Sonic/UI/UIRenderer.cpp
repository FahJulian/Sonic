#include <vector>
#include "Sonic/Base.h"
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Graphics2D/Texture.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Scene/PairView.h"
#include "Sonic/UI/Font/FontRenderer.h"
#include "Sonic/Scene/Scene.h"
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

static const int MAX_ELEMENTS = 5000;
static const int MAX_TEXTURES = 16;

static int s_ElementCount = 0;

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static Vertex s_Vertices[MAX_ELEMENTS * 4];
static std::vector<Sonic::Texture> s_Textures;


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
			s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\ui.vs", SONIC_RESOURCE_DIR + "shaders\\ui.fs");
			s_Shader.Bind();
			s_Shader.UniformFloat("u_WindowWidth", Window::getWidth());
			s_Shader.UniformFloat("u_WindowHeight", Window::getHeight());

			int textureSlots[MAX_TEXTURES];
			for (int i = 0; i < MAX_TEXTURES; i++)
				textureSlots[i] = i;

			s_Shader.UniformIntArray("u_Textures", textureSlots, MAX_TEXTURES);
			s_Shader.Unbind();

			s_Textures.reserve(MAX_TEXTURES);

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
		}

		void drawElement(int index, float x, float y, float zIndex, float width, float height, const Sprite& sprite, const Color& color, float borderWeight, const Color& borderColor, float edgeRadius)
		{
			float textureSlot = sprite.IsNull() ? -1.0f : textureSlotOf(*sprite.texture);

			Vertex* vertex = s_Vertices + 4 * index;
			for (int i = 0; i < 4; i++)
			{
				vertex->x = x + (i % 2) * width;
				vertex->y = y + (i / 2) * height;
				vertex->zIndex = zIndex;
				vertex->r = color.r;
				vertex->g = color.g;
				vertex->b = color.b;
				vertex->a = color.a;
				vertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
				vertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
				vertex->textureSlot = textureSlot;

				vertex->rectX = x;
				vertex->rectY = y;
				vertex->rectWidth = width;
				vertex->rectHeight = height;

				vertex->borderR = borderColor.r;
				vertex->borderG = borderColor.g;
				vertex->borderB = borderColor.b;
				vertex->borderA = borderColor.a;
				vertex->borderWeight = borderWeight;
				vertex->edgeRadius = edgeRadius;

				vertex++;
			}
		}

		void drawEntity(Scene* scene, EntityID e, UIRendererComponent* r, int index)
		{
			auto* c = scene->GetComponent<UIComponent>(e);

			const Sprite* sprite = r->GetSprite();
			const Color* color = r->GetColor();
			const Color* borderColor = color;
			float borderWeight = 0;
			float edgeRadius = 0;

			if (scene->HasComponent<UIHoverComponent>(e))
			{
				auto* h = scene->GetComponent<UIHoverComponent>(e);

				if (scene->HasComponent<ResizableComponent>(e))
				{
					auto* r = scene->GetComponent<ResizableComponent>(e);
					if (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left)
						h->SetHoverered(true);
				}

				if (h->IsHovered())
				{
					sprite = h->GetSprite();
					color = h->GetColor();
				}
			}

			if (scene->HasComponent<UIBorderComponent>(e))
			{
				auto* b = scene->GetComponent<UIBorderComponent>(e);
				borderColor = &b->color;
				borderWeight = b->weight;
			}

			if (scene->HasComponent<UIRoundedEdgeComponent>(e))
			{
				auto* r = scene->GetComponent<UIRoundedEdgeComponent>(e);
				edgeRadius = r->edgeRadius;
			}

			drawElement(index, c->GetX(), c->GetY(), c->GetZIndex(), c->GetWidth(), c->GetHeight(), *sprite, *color, borderWeight, *borderColor, edgeRadius);
		}

		void update(Scene* scene)
		{
			bool rebuffer = false;

			auto& entities = scene->View<UIRendererComponent>();
			s_ElementCount = entities.Size();

			int i = 0;
			for (auto [e, r] : entities)
			{
				if (i > MAX_ELEMENTS)
					return;

				if (*r->dirty)
				{
					drawEntity(scene, e, r, i);
					*r->dirty = false;
					rebuffer = true;
				}

				i++;
			}

			if (rebuffer)
			{
				s_VBO.Bind();
				s_VBO.SetData(s_Vertices, 4 * s_ElementCount * sizeof(Vertex));
				s_VBO.Unbind();
			}
		}

		void render()
		{
			s_Shader.Bind();
			s_VAO.Bind();

			for (int i = 0; i < s_Textures.size(); i++)
				s_Textures.at(i).Bind(i);

			glDrawElements(GL_TRIANGLES, s_ElementCount * 6, GL_UNSIGNED_INT, nullptr);

			for (int i = 0; i < s_Textures.size(); i++)
				s_Textures.at(i).Unbind();

			s_VAO.Unbind();
			s_Shader.Unbind();
		}

	}

}
