#pragma once
#include <GL/glew.h>
#include <memory>
#include <utility>
#include <vector>
#include <iostream>

namespace Sonic {
    
    /**
    * A ShaderDataType is one of the data types that exist 
    * in glsl
    */
    enum class ShaderDataType
    {
        Float, Float2, Float3, Float4,
        Int, Int1, Int2, Int3, Int4,
        Bool, Bool1, Bool2, Bool3, Bool4
    };

    /**
    * A Buffer Layout element is part of a Buffer layout. This layout is 
    * used for the vertex attrib pointers. Each buffer layout element will
    * result in one vertex attrib pointer
    */
    struct BufferLayoutElement
    {
        ShaderDataType m_Type;
        int m_Offset;
        bool m_Normalized;

        /**
        * Constructs a new BufferLayoutElement
        * 
        * @param type The Data type of this element
        * @param normalized Whether or not the data is normalized
        */
        BufferLayoutElement(ShaderDataType type, bool normalized = false)
            : m_Type(type), m_Offset(0), m_Normalized(normalized) {}
    };

    /**
    * A BufferLayout is used by the VertexArray to adjust its vertex attrib
    * pointers when a new VertexBuffer is added.
    */
    struct BufferLayout
    {
    private:
        std::vector<BufferLayoutElement> m_Elements;

    public:
        /**
        * Constructs a new BufferLayout
        * 
        * @param elements The elements of this BufferLayout
        */
        BufferLayout(const std::initializer_list<BufferLayoutElement>& elements)
            : m_Elements(elements) {}

    private:
        friend class VertexBuffer;
        friend class VertexArray;
    };

    /**
    * A VertexBuffer is an opengl vbo. It does not hold the data but is 
    * just used to be a wrapper around opengl functions to upload the 
    * data to the gpu.
    *
    * It holds its data layout in form of a BufferLayout, which is used
    * by the VertexArray to adjust its vertex attrib pointers when the
    * VertexBuffer is added to it.
    * 
    * The VertexBuffer has two subclasses: The StaticVertexBuffer 
    * and the DynamicVertexBuffer.
    * The StaticVertexBuffer is given its data at construction, and 
    * directly uploads it to the gpu. The data can not be changed later.
    * The DynamicVertexBuffer is not given its data at constructon but
    * has a function to upload data to the gpu that can be used as
    * often as needed.
    */
    class VertexBuffer
    {
    protected:
        /**
        * Constructs a new VertexBuffer
        * 
        * @param layout The data layout of this vertex buffer
        */
        VertexBuffer(const BufferLayout& layout);
    public:
        /**
        * Binds this VertexBuffer to the GL_ARRAY_BUFFER slot
        */
        void Bind() const;

        /**
        * Unbinds all VertexBuffers from GL_ARRAY_BUFFER
        */
        void Unbind() const;
    private:
        BufferLayout m_Layout;
        unsigned int m_OpenGL_ID;
        int m_Stride;

        friend class VertexArray;
    };

    /**
    * The StaticVertexBuffer is given its data at construction, and 
    * directly uploads it to the gpu. The data can not be changed later.
    */
    class StaticVertexBuffer : public VertexBuffer
    {
    public:
        StaticVertexBuffer(const void* data, int size, const BufferLayout& layout);
    };

    /** 
    * The DynamicVertexBuffer is not given its data at constructon but
    * has a function to upload data to the gpu that can be used as
    * often as needed.
    */
    class DynamicVertexBuffer : public VertexBuffer
    {
    public:
        DynamicVertexBuffer(int maxSize, const BufferLayout& layout);

        void SetData(const void* data, int size);
    };

    /**
    * Get the size in bytes of the given shader data type
    * 
    * @param type The ShaderDataType to check
    * 
    * @return The size in bytes of the given data type
    */
    static int sizeOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return 1 * 4;
            case ShaderDataType::Float2: return 2 * 4;
            case ShaderDataType::Float3: return 3 * 4;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Int: return 1 * 4;
            case ShaderDataType::Int2: return 2 * 4;
            case ShaderDataType::Int3: return 3 * 4;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1 * 1;
            case ShaderDataType::Bool2: return 2 * 1;
            case ShaderDataType::Bool3: return 3 * 1;
            case ShaderDataType::Bool4: return 4 * 1;
        }

        return -1;
    }

    /**
    * Get the opengl enum for the primitive type of the
    * given shader data type
    * 
    * @param type The ShaderDataType to check
    * 
    * @return The primitive type of the given data type
    */
    static int primitiveTypeOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::Bool2: return GL_BOOL;
            case ShaderDataType::Bool3: return GL_BOOL;
            case ShaderDataType::Bool4: return GL_BOOL;
        }

        return -1;
    }

    /**
    * Get the amount of components that are in a given
    * shader data type.
    * 
    * @param type The ShaderDataType to check
    *
    * @return The amount of components in this shader data type
    */
    static int componentCountOfShaderType(ShaderDataType type)
    {
        switch (type)
        {
            
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Bool2: return 2;
            case ShaderDataType::Bool3: return 3;
            case ShaderDataType::Bool4: return 4;
        }

        return -1;
    }
    
}
