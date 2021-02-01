#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Base.h"
#include "Sonic/Graphics/Buffer/VertexArray.h"
#include "Sonic/Graphics/Buffer/VertexBuffer.h"
#include "Sonic/Graphics/Graphics2D/Texture.h"
#include "Sonic/Graphics/Shader.h"
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

static Sonic::Shader s_Shader = Sonic::Shader::Null();
static Sonic::VertexBuffer s_VBO = Sonic::VertexBuffer::Null();
static Sonic::VertexArray s_VAO = Sonic::VertexArray::Null();

static int s_RectCount;
static Vertex s_Vertices[MAX_RECTS * 4];
static Vertex* s_NextVertex;

static std::vector<Sonic::Texture> s_Textures;
static int s_TextureSlots[16];


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
            s_RectCount = 0;
            s_NextVertex = &s_Vertices[0];

            s_Shader = Shader(SONIC_RESOURCE_DIR + "shaders\\rectangle.vs", SONIC_RESOURCE_DIR + "shaders\\rectangle.fs");

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

            s_Textures.reserve(MAX_TEXTURES);
            for (int i = 0; i < MAX_TEXTURES; i++)
                s_TextureSlots[i] = i;

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            if (s_RectCount == MAX_RECTS)
                return;

            float textureSlot = sprite.IsNull() ? -1 : textureSlotOf(*sprite.texture);

            if (rotation == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    s_NextVertex->x = position.x + (i % 2) * size.x;
                    s_NextVertex->y = position.y + (i / 2) * size.y;
                    s_NextVertex->z = position.z;
                    s_NextVertex->r = color.r;
                    s_NextVertex->g = color.g;
                    s_NextVertex->b = color.b;
                    s_NextVertex->a = color.a;
                    s_NextVertex->textureX = i % 2 == 0 ? sprite.x0 : sprite.x1;
                    s_NextVertex->textureY = i / 2 == 0 ? sprite.y0 : sprite.y1;
                    s_NextVertex->textureSlot = textureSlot;

                    s_NextVertex++;
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

            s_RectCount++;
        }

        void startScene(const Camera2D* camera)
        {
            s_NextVertex = &s_Vertices[0];
            s_RectCount = 0;

            s_Shader.Bind();
            s_Shader.UniformMat4("u_ViewMatrix", camera->GetView());
            s_Shader.UniformMat4("u_ProjectionMatrix", camera->GetProjection());
            s_Shader.UniformIntArray("u_Textures", s_TextureSlots, MAX_TEXTURES);
        }

        void endScene()
        {
            s_Shader.Bind();
            s_VBO.SetData(reinterpret_cast<float*>(s_Vertices), 4 * s_RectCount * sizeof(Vertex));

            s_VAO.Bind();

            for (int i = 0; i < s_Textures.size(); i++)
                s_Textures.at(i).Bind(i);

            glDrawElements(GL_TRIANGLES, s_RectCount * 6, GL_UNSIGNED_INT, nullptr);

            for (int i = 0; i < s_Textures.size(); i++)
                s_Textures.at(i).Unbind();

            s_Shader.Unbind();
            s_VAO.Unbind();
        }

        void setClearColor(const Color& color) 
        { 
            glClearColor(color.r, color.g, color.b, color.a); 
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect(position, size, 0.0f, sprite, color);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, sprite, color);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, sprite, color);
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRect(position, size, rotation, Sprite(), color);
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color)
        {
            drawRect(position, size, 0.0f, Sprite(), color);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, Sprite(), color);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Color& color)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, Sprite(), color);
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRect(position, size, rotation, sprite, WHITE);
        }

        void drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(position, size, 0.0f, sprite, WHITE);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, float rotation, const Sprite& sprite)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, rotation, sprite, WHITE);
        }

        void drawRect(const glm::vec2& position, const glm::vec2& size, const Sprite& sprite)
        {
            drawRect(glm::vec3{ position, 0.0f }, size, 0.0f, sprite, WHITE);
        }

    }

}
