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
#include "Renderer2D.h"

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

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static Vertex s_Vertices[MAX_RECTS * 4];
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

    namespace Renderer2D {

        void init()
        {
            s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\rectangle.vs", SONIC_RESOURCE_DIR + "shaders\\rectangle.fs");
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

        void drawRect(int index, const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            float textureSlot = sprite.IsNull() ? -1 : textureSlotOf(*sprite.texture);

            Vertex* vertex = s_Vertices + 4 * index;
            if (rotation == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    vertex->x = position.x + (i % 2) * size.x;
                    vertex->y = position.y + (i / 2) * size.y;
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
                //for (int i = 0; i < 4; i++)
                //{
                //    s_Data.nextVertex->x = position.x + (i % 2) * size.x;
                //    s_Data.nextVertex->y = position.y + (i / 2) * size.y;
                //    s_Data.nextVertex->z = position.z;
                //    s_Data.nextVertex->r = color.r;
                //    s_Data.nextVertex->g = color.g;
                //    s_Data.nextVertex->b = color.b;
                //    s_Data.nextVertex->a = color.a;
                //    s_Data.nextVertex->textureX = sprite.GetTextureCoords()[2 * i + 0];
                //    s_Data.nextVertex->textureY = sprite.GetTextureCoords()[2 * i + 1];
                //    s_Data.nextVertex->textureSlot = textureSlot;

                //    s_Data.nextVertex++;
                //}
            }
        }

        void startScene(const Camera2D* camera)
        {
            s_RectCount = 0;

        }

        void drawEntity(Scene* scene, EntityID e, Renderer2DComponent* r, int index)
        {
            auto* t = scene->GetComponent<Transform2DComponent>(e);
            drawRect(index, t->GetPosition(), t->GetScale(), t->GetRotation(), r->GetSprite(), r->GetColor());
        }

        void update(Scene* scene, const Camera2D* camera)
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

}
