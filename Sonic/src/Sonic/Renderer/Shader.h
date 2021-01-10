#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Color.h"

namespace Sonic {

    class Shader
    {
    public:
        /**
        * Constructs an OpenGL shader program with a vertex shader and a fragment shader.
        * 
        * @param vertexSourcePath Path to the .vs file containing the vertex shader source code.
        * @param fragmentSourcePath Path to the .fs file containing the fragment shader source code.
        */
        Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);

        /**
        * Binds this shader program to OpenGL
        */
        void Bind();

        /**
        * Unbinds all shader programs from OpenGL
        */
        void Unbind();

        /**
        * Uploads an integer to the specified uniform variable
        * 
        * @param name The name of the uniform variable
        * @param value The integer value to upload to the uniform
        */
        void UniformInt(const std::string& name, int value);

        /**
        * Uploads an integer array to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param values Pointer to the first integer in the array
        * @param count Size of the array
        */
        void UniformIntArray(const std::string& name, const int* values, int count);

        /**
        * Uploads a float to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The float value to upload to the uniform
        */
        void UniformFloat(const std::string& name, float value);

        /**
        * Uploads a float2 vector to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The float2 vector value to upload to the uniform
        */
        void UniformFloat2(const std::string& name, const glm::vec2& value);

        /**
        * Uploads a float3 vector to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The float3 vector value to upload to the uniform
        */
        void UniformFloat3(const std::string& name, const glm::vec3& value);

        /**
        * Uploads a float4 vector to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The float4 vector value to upload to the uniform
        */
        void UniformFloat4(const std::string& name, const glm::vec4& value);

        /**
        * Uploads an Color to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The Color value to upload to the uniform
        */
        void UniformFloat4(const std::string& name, const Color& value);

        /**
        * Uploads a mat4 matrix to the specified uniform variable
        *
        * @param name The name of the uniform variable
        * @param value The mat4 matrix value to upload to the uniform
        */
        void UniformMat4(const std::string& name, const glm::mat4& value);
    private:
        unsigned int m_OpenGL_ID;
    };

}
