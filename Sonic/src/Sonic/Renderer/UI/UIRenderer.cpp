#include <gl/glew.h>
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Texture.h"
#include "Sonic/Graphics/Sprite.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/Views.h"
#include "Sonic/Scene/Components/UIComponents.h"
#include "UIRenderer.h"

using namespace Sonic;


struct UIVertex
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

static const int MAX_ELEMENTS = 20000;
static const int MAX_TEXTURES = 16;

static int s_ElementCount = 0;

static Shader s_Shader = Shader::Null();
static VertexBuffer s_VBO = VertexBuffer::Null();
static VertexArray s_VAO = VertexArray::Null();

static UIVertex s_Vertices[MAX_ELEMENTS * 4];
static std::vector<Texture> s_Textures;

static bool s_CompleteRebuffer = false;


static float textureSlotOf(const Texture& texture)
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


void UIRenderer::init()
{
	s_Shader = Shader(coreResourceDir() + "shaders\\ui.vs", coreResourceDir() + "shaders\\ui.fs");
	s_Shader.Bind();
	s_Shader.UniformFloat("u_ViewportWidth", Window::getWidth());
	s_Shader.UniformFloat("u_ViewportHeight", Window::getHeight());

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

	s_VBO = Sonic::VertexBuffer(4 * MAX_ELEMENTS * sizeof(UIVertex), { 3, 4, 2, 1, 2, 2, 4, 1, 1 });
	s_VAO = Sonic::VertexArray(indices, MAX_ELEMENTS * 6, { s_VBO });

	delete[] indices;
}

void UIRenderer::drawElement(int index, float x, float y, float width, float height, float zIndex, const UIRendererProperties* properties)
{
	x = (float)((int)x);
	y = (float)((int)y);
	width = (float)((int)width);
	height = (float)((int)height);

	float textureSlot = properties->sprite.IsNull() ? -1.0f : textureSlotOf(properties->sprite.texture);

	UIVertex* vertex = s_Vertices + 4 * static_cast<size_t>(index);
	for (int i = 0; i < 4; i++)
	{
		vertex->x = x + (i % 2) * width;
		vertex->y = y + (i / 2) * height;
		vertex->zIndex = zIndex;
		vertex->r = properties->color.r;
		vertex->g = properties->color.g;
		vertex->b = properties->color.b;
		vertex->a = properties->color.a;
		vertex->textureX = i % 2 == 0 ? properties->sprite.x0 : properties->sprite.x1;
		vertex->textureY = i / 2 == 0 ? properties->sprite.y0 : properties->sprite.y1;
		vertex->textureSlot = textureSlot;
									 
		vertex->rectX = x;
		vertex->rectY = y;

		vertex->rectWidth = width;
		vertex->rectHeight = height;

		vertex->borderR = properties->borderColor.r;
		vertex->borderG = properties->borderColor.g;
		vertex->borderB = properties->borderColor.b;
		vertex->borderA = properties->borderColor.a;
		vertex->borderWeight = (float)properties->borderWeight;
		vertex->edgeRadius = (float)properties->edgeRadius;

		vertex++;
	}
}

void UIRenderer::rebuffer(Scene* scene)
{
	bool rebuffer = false;

	auto entities = scene->View<UIRendererComponent>();
	s_ElementCount = entities.Size();
	if (s_ElementCount > MAX_ELEMENTS)
		s_ElementCount = MAX_ELEMENTS;

	if (s_CompleteRebuffer)
	{
		int i = 0;
		for (auto [e, r] : entities)
		{
			if (i > MAX_ELEMENTS)
				break;

			auto* c = scene->GetComponent<UIComponent>(e);
			const UIRendererProperties* properties = nullptr;

			if (UIHoverComponent* h = nullptr;
				scene->HasComponent<UIHoverComponent>(e) && (h = scene->GetComponent<UIHoverComponent>(e))->hovered)
			{
				properties = &h->properties;
			}
			else
			{
				properties = &r->properties;
			}

			drawElement(i, c->x, c->y, c->width, c->height, c->zIndex, properties);

			*r->dirty = false;
			rebuffer = true;

			i++;
		}
	}
	else
	{
		int i = 0;
		for (auto [e, r] : entities)
		{
			if (i > MAX_ELEMENTS)
				break;

			if (*r->dirty)
			{
				auto* c = scene->GetComponent<UIComponent>(e);
				const UIRendererProperties* properties = nullptr;

				if (UIHoverComponent* h = nullptr;
					scene->HasComponent<UIHoverComponent>(e) && (h = scene->GetComponent<UIHoverComponent>(e))->hovered)
				{
					properties = &h->properties;
				}
				else
				{
					properties = &r->properties;
				}

				drawElement(i, c->x, c->y, c->width, c->height, c->zIndex, properties);

				*r->dirty = false;
				rebuffer = true;
			}

			i++;
		}
	}

	if (rebuffer)
	{
		s_VBO.Bind();
		s_VBO.SetData(s_Vertices, 4 * s_ElementCount * sizeof(UIVertex));
		s_VBO.Unbind();

		s_CompleteRebuffer = false;
	}
}

void UIRenderer::render()
{
	if (s_ElementCount == 0)
		return;

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

void UIRenderer::setViewportSize(float width, float height)
{
	s_Shader.Bind();
	s_Shader.UniformFloat("u_ViewportWidth", width);
	s_Shader.UniformFloat("u_ViewportHeight", height);
	s_Shader.Unbind();

	s_CompleteRebuffer = true;
}

void UIRenderer::markDirty()
{
	s_CompleteRebuffer = true;
}

void UIRenderer::destroy()
{
	s_Textures.clear();
}
