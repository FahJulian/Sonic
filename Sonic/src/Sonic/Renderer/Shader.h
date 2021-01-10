#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Color.h"

namespace Sonic {

    class Shader
    {
    public:
        Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);

        void Bind();
        void Unbind();

        void UniformInt(const std::string& name, int value);
        void UniformIntArray(const std::string& name, const int* values, int count);
        void UniformFloat(const std::string& name, float value);
        void UniformFloat2(const std::string& name, const glm::vec2& value);
        void UniformFloat3(const std::string& name, const glm::vec3& value);
        void UniformFloat4(const std::string& name, const glm::vec4& value);
        void UniformFloat4(const std::string& name, const Color& value);
        void UniformMat4(const std::string& name, const glm::mat4& value);
    private:
        unsigned int m_OpenGL_ID;
    };

}
