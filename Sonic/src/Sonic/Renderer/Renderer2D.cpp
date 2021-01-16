#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Sonic/Base.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Texture.h"

static const int MAX_RECTS = 10000;
static const int MAX_TEXTURES = 16;

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
    float textureX, textureY;
    float textureSlot;
};

struct QuadRendererData
{
    Sonic::VertexBuffer vertexBuffer;
    Sonic::VertexArray vertexArray;

    int rectCount;
    Vertex vertices[MAX_RECTS * 4];
    Vertex* nextVertex = &vertices[0];

    std::vector<Sonic::Texture> textures;
    int textureSlots[16];

    QuadRendererData()
        : vertexBuffer(Sonic::VertexBuffer::Null()), vertexArray(Sonic::VertexArray::Null()),
          rectCount(0), vertices{ 0 }, nextVertex(&vertices[0]) {}

    void Init()
    {
        for (int i = 0; i < MAX_TEXTURES; i++)
            textureSlots[i] = i;

        int indices[6 * MAX_RECTS];
        for (int i = 0; i < MAX_RECTS; i++)
        {
            indices[6 * i + 0] = 4 * i + 0;
            indices[6 * i + 1] = 4 * i + 1;
            indices[6 * i + 2] = 4 * i + 2;

            indices[6 * i + 3] = 4 * i + 1;
            indices[6 * i + 4] = 4 * i + 3;
            indices[6 * i + 5] = 4 * i + 2;
        }

        vertexBuffer = Sonic::VertexBuffer(4 * MAX_RECTS * sizeof(Vertex), { 3, 4, 2, 1 });
        vertexArray = Sonic::VertexArray(indices, MAX_RECTS * 6, { vertexBuffer });

        textures.reserve(MAX_TEXTURES);
    }
};


static QuadRendererData s_Data;
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

    return 0.0f;
}


namespace Sonic {

    void Renderer2D::init()
    {
        s_Data.Init();
        s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\rectangle.vs", SONIC_RESOURCE_DIR + "shaders\\rectangle.fs");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
    {
        if (s_Data.rectCount == MAX_RECTS)
            return;

        float textureSlot = sprite.IsNull() ? -1 : textureSlotOf(*sprite.GetTexture());

        if (rotation == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                s_Data.nextVertex->x = position.x + (i % 2) * size.x;
                s_Data.nextVertex->y = position.y + (i / 2) * size.y;
                s_Data.nextVertex->z = position.z;
                s_Data.nextVertex->r = color.r;
                s_Data.nextVertex->g = color.g;
                s_Data.nextVertex->b = color.b;
                s_Data.nextVertex->a = color.a;
                s_Data.nextVertex->textureX = sprite.GetTextureCoords()[2 * i + 0];
                s_Data.nextVertex->textureY = sprite.GetTextureCoords()[2 * i + 1];
                s_Data.nextVertex->textureSlot = textureSlot;

                s_Data.nextVertex++;
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                s_Data.nextVertex->x = position.x + (i % 2) * size.x;
                s_Data.nextVertex->y = position.y + (i / 2) * size.y;
                s_Data.nextVertex->z = position.z;
                s_Data.nextVertex->r = color.r;
                s_Data.nextVertex->g = color.g;
                s_Data.nextVertex->b = color.b;
                s_Data.nextVertex->a = color.a;
                s_Data.nextVertex->textureX = sprite.GetTextureCoords()[2 * i + 0];
                s_Data.nextVertex->textureY = sprite.GetTextureCoords()[2 * i + 1];
                s_Data.nextVertex->textureSlot = textureSlot;

                s_Data.nextVertex++;
            }
        }

        s_Data.rectCount++;
    }

    void Renderer2D::startScene(const Camera2D* camera)
    {
        s_Data.nextVertex = &s_Data.vertices[0];
        s_Data.rectCount = 0;

        s_Shader.Bind();
        s_Shader.UniformMat4("u_ViewMatrix", camera->GetView());
        s_Shader.UniformMat4("u_ProjectionMatrix", camera->GetProjection());
        s_Shader.UniformIntArray("u_Textures", s_Data.textureSlots, MAX_TEXTURES);
    }

    void Renderer2D::endScene()
    {
        s_Data.vertexBuffer.SetData(reinterpret_cast<float*>(s_Data.vertices), 4 * s_Data.rectCount * sizeof(Vertex));

        s_Data.vertexArray.Bind();

        for (int i = 0; i < s_Data.textures.size(); i++)
            s_Data.textures.at(i).Bind(i);

        glDrawElements(GL_TRIANGLES, s_Data.rectCount * 6, GL_UNSIGNED_INT, nullptr);

        for (int i = 0; i < s_Data.textures.size(); i++)
            s_Data.textures.at(i).Unbind();

        s_Shader.Unbind();
        s_Data.vertexArray.Unbind();
    }

}
