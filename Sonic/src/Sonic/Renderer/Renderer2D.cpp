#include <memory>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "../Base.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Texture.h"
#include "buffer/VertexArray.h"
#include "buffer/VertexBuffer.h"

static const int MAX_TEXTURES = 16;

struct QuadVertex
{
    float x, y, z;
    float r, g, b, a;
    float textureX, textureY;
    float textureSlot;
};

struct QuadRendererData
{
    static const int maxQuads = 10000;

    std::unique_ptr<Sonic::VertexArray> vertexArray;
    std::unique_ptr<Sonic::DynamicVertexBuffer> vertexBuffer;

    int quadCount = 0;
    QuadVertex vertices[maxQuads * 4];
    QuadVertex* nextQuadPtr = &vertices[0];

    std::vector<Sonic::Texture> textures;
    const int textureSlots[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    void Init()
    {
        vertexArray = std::make_unique<Sonic::VertexArray>();
        vertexBuffer = std::unique_ptr<Sonic::DynamicVertexBuffer>(new Sonic::DynamicVertexBuffer(4 * maxQuads * sizeof(QuadVertex), {
            { Sonic::ShaderDataType::Float3 },
            { Sonic::ShaderDataType::Float4 },
            { Sonic::ShaderDataType::Float2 },
            { Sonic::ShaderDataType::Float  }
        }));
        vertexArray->AddVertexBuffer(*vertexBuffer);

        textures.reserve(MAX_TEXTURES);

        int* quadIndices = new int[maxQuads * 6];
        for (int i = 0; i < maxQuads; i++)
        {
            quadIndices[6 * i + 0] = 4 * i + 0;
            quadIndices[6 * i + 1] = 4 * i + 1;
            quadIndices[6 * i + 2] = 4 * i + 2;

            quadIndices[6 * i + 3] = 4 * i + 1;
            quadIndices[6 * i + 4] = 4 * i + 3;
            quadIndices[6 * i + 5] = 4 * i + 2;
        }

        vertexArray->SetIndexBuffer(std::shared_ptr<Sonic::IndexBuffer>(new Sonic::IndexBuffer(quadIndices, 6 * maxQuads)));
        delete[] quadIndices;
    }
};

static QuadRendererData s_Data;
static std::unique_ptr<Sonic::Shader> s_QuadShader; 
static const Sonic::Camera2D* s_Camera;

static int textureSlotOf(const Sonic::Texture& texture)
{
    for (int i = 0; i < s_Data.textures.size(); i++)
    {
        if (s_Data.textures.at(i) == texture)
            return i;
    }

    if (s_Data.textures.size() < MAX_TEXTURES)
    {
        s_Data.textures.push_back(texture);
        return static_cast<int>(s_Data.textures.size()) - 1;
    }

    return -1;
}

namespace Sonic {

    void Renderer2D::init()
    {
        s_Data.Init();
        s_QuadShader = std::unique_ptr<Shader>(new Shader(SONIC_RESOURCE_DIR + "shaders\\rectangle.vs", SONIC_RESOURCE_DIR + "shaders\\rectangle.fs"));
        s_Camera = nullptr;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const Color& color)
    {
        if (s_Data.quadCount == s_Data.maxQuads)
            return;

        for (int i = 0; i < 4; i++)
        {
            s_Data.nextQuadPtr->x = position.x + (i % 2) * size.x;
            s_Data.nextQuadPtr->y = position.y + (i / 2) * size.y;
            s_Data.nextQuadPtr->z = position.z;
            s_Data.nextQuadPtr->r = color.r;
            s_Data.nextQuadPtr->g = color.g;
            s_Data.nextQuadPtr->b = color.b;
            s_Data.nextQuadPtr->a = color.a;
            s_Data.nextQuadPtr->textureX = 0.0f;
            s_Data.nextQuadPtr->textureY = 0.0f;
            s_Data.nextQuadPtr->textureSlot = -1;

            s_Data.nextQuadPtr++;
        }

        s_Data.quadCount++;
    }

    void Renderer2D::drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Color& color)
    {
        if (s_Data.quadCount == s_Data.maxQuads)
            return;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        glm::vec4 QuadVertexPositions[4];
        QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        QuadVertexPositions[2] = { -0.5f,  0.5f, 0.0f, 1.0f };
        QuadVertexPositions[3] = {  0.5f,  0.5f, 0.0f, 1.0f };

        for (int i = 0; i < 4; i++)
        {
            const glm::vec3& pos = transform * QuadVertexPositions[i];
            s_Data.nextQuadPtr->x = pos.x; 
            s_Data.nextQuadPtr->y = pos.y; 
            s_Data.nextQuadPtr->z = pos.z;
            s_Data.nextQuadPtr->r = color.r; 
            s_Data.nextQuadPtr->g = color.g; 
            s_Data.nextQuadPtr->b = color.b; 
            s_Data.nextQuadPtr->a = color.a; 
            s_Data.nextQuadPtr->textureX = 0.0f;
            s_Data.nextQuadPtr->textureY = 0.0f;
            s_Data.nextQuadPtr->textureSlot = -1;

            s_Data.nextQuadPtr++;
        }

        s_Data.quadCount++;
    }
    
    void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const Sprite& sprite, const Color& color)
    {
        if (s_Data.quadCount == s_Data.maxQuads)
            return;

        for (int i = 0; i < 4; i++)
        {
            s_Data.nextQuadPtr->x = position.x + (i % 2) * size.x;
            s_Data.nextQuadPtr->y = position.y + (i / 2) * size.y;
            s_Data.nextQuadPtr->z = position.z;
            s_Data.nextQuadPtr->r = color.r;
            s_Data.nextQuadPtr->g = color.g;
            s_Data.nextQuadPtr->b = color.b;
            s_Data.nextQuadPtr->a = color.a;
            s_Data.nextQuadPtr->textureX = sprite.GetTextureCoords()[2 * i + 0];
            s_Data.nextQuadPtr->textureY = sprite.GetTextureCoords()[2 * i + 1];
            s_Data.nextQuadPtr->textureSlot = static_cast<float>(textureSlotOf(sprite.GetTexture()));

            s_Data.nextQuadPtr++;
        }

        s_Data.quadCount++;
    }

    void Renderer2D::drawRotatedRect(const glm::vec3& position, const glm::vec2& size, float rotation, const Sprite& sprite, const Color& color)
    {
        if (s_Data.quadCount == s_Data.maxQuads)
            return;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        glm::vec4 QuadVertexPositions[4];
        QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        QuadVertexPositions[2] = { -0.5f,  0.5f, 0.0f, 1.0f };
        QuadVertexPositions[3] = {  0.5f,  0.5f, 0.0f, 1.0f };

        int textureSlot = textureSlotOf(sprite.GetTexture());

        for (int i = 0; i < 4; i++)
        {
            const glm::vec3& pos = transform * QuadVertexPositions[i];
            s_Data.nextQuadPtr->x = pos.x; 
            s_Data.nextQuadPtr->y = pos.y; 
            s_Data.nextQuadPtr->z = pos.z;
            s_Data.nextQuadPtr->r = color.r; 
            s_Data.nextQuadPtr->g = color.g; 
            s_Data.nextQuadPtr->b = color.b; 
            s_Data.nextQuadPtr->a = color.a; 
            s_Data.nextQuadPtr->textureX = sprite.GetTextureCoords()[2 * i + 0];
            s_Data.nextQuadPtr->textureY = sprite.GetTextureCoords()[2 * i + 1];
            s_Data.nextQuadPtr->textureSlot = static_cast<float>(textureSlotOf(sprite.GetTexture()));

            s_Data.nextQuadPtr++;
        }

        s_Data.quadCount++;
    }
    // User32.lib;Gdi32.lib;Shell32.lib;
    void Renderer2D::startScene(const Camera2D* camera)
    {
        s_Camera = camera;
        
        s_Data.nextQuadPtr = &s_Data.vertices[0];
        s_Data.quadCount = 0;

        s_QuadShader->Bind();
        s_QuadShader->UniformMat4("u_ViewMatrix", s_Camera->GetView());
        s_QuadShader->UniformMat4("u_ProjectionMatrix", s_Camera->GetProjection());
        s_QuadShader->UniformIntArray("u_Textures", s_Data.textureSlots, 16);
    }

    void Renderer2D::endScene()
    {
        s_Data.vertexBuffer->SetData(&s_Data.vertices[0], 4 * s_Data.quadCount * sizeof(QuadVertex));

        s_Data.vertexArray->Bind();

        for (int i = 0; i < s_Data.textures.size(); i++)
            s_Data.textures.at(i).Bind(i);

        glDrawElements(GL_TRIANGLES, s_Data.quadCount * 6, GL_UNSIGNED_INT, nullptr);

        for (int i = 0; i < s_Data.textures.size(); i++)
            s_Data.textures.at(i).Unbind();

        s_QuadShader->Unbind();
        s_Data.vertexArray->Unbind();
    }

}
