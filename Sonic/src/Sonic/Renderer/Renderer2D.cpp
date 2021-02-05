#include <math.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Base.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Graphics2D/Texture.h"
#include "Sonic/Graphics/Shader.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/PairView.h"
#include "Sonic/Scene/Components.h"
#include "Renderer2D.h"

using namespace Sonic;


static const float PI = 3.14159265358979323846f;

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
    float textureX, textureY;
    float textureSlot;
};

static const int MAX_RECTS = 10000;
static const int MAX_TEXTURES = 16;

static int s_RectCount;

static Shader s_Shader = Shader::Null();
static VertexBuffer s_VBO = VertexBuffer::Null();
static VertexArray s_VAO = VertexArray::Null();

static Vertex s_Vertices[MAX_RECTS * 4];
static std::vector<Texture> s_Textures;


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


namespace Sonic::Renderer2D {

    void init()
    {
        s_Shader = Shader(coreResourceDir() + "shaders\\rectangle.vs", coreResourceDir() + "shaders\\rectangle.fs");
        s_Shader.Bind();

        int textureSlots[MAX_TEXTURES];
        for (int i = 0; i < MAX_TEXTURES; i++)
            textureSlots[i] = i;

        s_Shader.UniformIntArray("u_Textures", textureSlots, MAX_TEXTURES);
        s_Shader.Unbind();

        s_Textures.reserve(MAX_TEXTURES);

        int* indices = new int[6 * MAX_RECTS];
        for (int i = 0; i < MAX_RECTS; i++)
        {
            indices[6 * i + 0] = 4 * i + 0;
            indices[6 * i + 1] = 4 * i + 1;
            indices[6 * i + 2] = 4 * i + 2;

            indices[6 * i + 3] = 4 * i + 1;
            indices[6 * i + 4] = 4 * i + 3;
            indices[6 * i + 5] = 4 * i + 2;
        }

        s_VBO = Sonic::VertexBuffer(4 * MAX_RECTS * sizeof(Vertex), { 3, 4, 2, 1 });
        s_VAO = Sonic::VertexArray(indices, MAX_RECTS * 6, { s_VBO });

        delete[] indices;
    }

    void drawRect(int index, const glm::vec3& position, const glm::vec2& scale, float rotation, const Sprite& sprite, const Color& color)
    {
        float textureSlot = sprite.IsNull() ? -1 : textureSlotOf(*sprite.texture);

        Vertex* vertex = s_Vertices + 4 * (size_t)index;
        if (rotation == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                vertex->x = position.x + (i % 2) * scale.x;
                vertex->y = position.y + (i / 2) * scale.y;
                vertex->z = position.z;
                vertex->r = color.r;
                vertex->g = color.g;
                vertex->b = color.b;
                vertex->a = color.a;
                vertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
                vertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
                vertex->textureSlot = textureSlot;

                vertex++;
            }
        }
        else
        {
            glm::vec2 center = { 0.5f * scale.x, 0.5f * scale.y };
            float hypo = std::sqrt(center.x * center.x + center.y * center.y);

            float sin = std::sin(rotation * PI / 180) * hypo;
            float cos = std::cos(rotation * PI / 180) * hypo;

            vertex[0].x = position.x + center.x - sin;
            vertex[0].y = position.y + center.y - cos;

            vertex[1].x = position.x + center.x + cos;
            vertex[1].y = position.y + center.y - sin;

            vertex[2].x = position.x + center.x - cos;
            vertex[2].y = position.y + center.y + sin;

            vertex[3].x = position.x + center.x + sin;
            vertex[3].y = position.y + center.y + cos;

            for (int i = 0; i < 4; i++)
            {
                vertex->z = position.z;
                vertex->r = color.r;
                vertex->g = color.g;
                vertex->b = color.b;
                vertex->a = color.a;
                vertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
                vertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
                vertex->textureSlot = textureSlot;

                vertex++;
            }
        }
    }

    void drawEntity(Scene* scene, EntityID e, Renderer2DComponent* r, int index)
    {
        auto* t = scene->GetComponent<Transform2DComponent>(e);
        drawRect(index, t->GetPosition(), t->GetScale(), t->GetRotation(), r->GetSprite(), r->GetColor());
    }

    void rebuffer(Scene* scene, const Camera2D* camera)
    {
        s_Shader.Bind();
        s_Shader.UniformMat4("u_ViewMatrix", camera->GetView());
        s_Shader.UniformMat4("u_ProjectionMatrix", camera->GetProjection());
        s_Shader.Unbind();

        bool rebuffer = false;

        auto& entities = scene->View<Renderer2DComponent>();
        s_RectCount = entities.Size();

        int i = 0;
        for (auto [e, r] : entities)
        {
            if (i > MAX_RECTS)
                break;

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
            s_VBO.SetData(reinterpret_cast<float*>(s_Vertices), 4 * s_RectCount * sizeof(Vertex));
            s_VBO.Unbind();
        }
    }

    void render()
    {
        s_Shader.Bind();
        s_VAO.Bind();

        for (int i = 0; i < s_Textures.size(); i++)
            s_Textures.at(i).Bind(i);

        glDrawElements(GL_TRIANGLES, s_RectCount * 6, GL_UNSIGNED_INT, nullptr);

        for (int i = 0; i < s_Textures.size(); i++)
            s_Textures.at(i).Unbind();

        s_VAO.Unbind();
        s_Shader.Unbind();
    }

}
