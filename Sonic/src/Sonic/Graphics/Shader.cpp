#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Sonic/Debug/Log/Log.h"
#include "Shader.h"

using namespace Sonic;


struct ShaderCompilationErrorInfo
{
    bool compilationSuccessful;
    String infoLog;
};

static String loadFile(const String& filePath)
{
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static void loadSourceToShader(const unsigned int shader, const char* source, const int sourceLength)
{
    glShaderSource(shader, 1, &source, &sourceLength);
}

static ShaderCompilationErrorInfo compilationSuccessful(unsigned int programID, unsigned int vertexShaderID, unsigned int fragmentShaderID)
{
    int info;

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &info);
    if (info == GL_FALSE)
    {
        glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &info);
        char* infoLog = new char[info];
        glGetShaderInfoLog(vertexShaderID, info, nullptr, infoLog);

        String s = String("Vertex Shader Compilation Error:\n") + String(infoLog);
        delete[] infoLog;

        return { false, s };
    }

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &info);
    if (info == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &info);
        char* infoLog = new char[info];
        glGetShaderInfoLog(fragmentShaderID, info, nullptr, infoLog);

        String s = String("Fragment Shader Compilation Error:\n") + String(infoLog);
        delete[] infoLog;

        return { false, s };
    }

    glGetProgramiv(programID, GL_LINK_STATUS, &info);
    if (info == GL_FALSE)
    {
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &info);
        char* infoLog = new char[info];
        glGetProgramInfoLog(programID, info, nullptr, infoLog);

        String s = String("Shader Linking Error:\n") + String(infoLog);
        delete[] infoLog;

        return { false, s };
    }
    return { true, "" };
}


Shader::Shader(const String& vertexSourcePath, const String& fragmentSourcePath)
    : m_OpenGL_ID(std::make_shared<unsigned int>(0))
{
    const String& vertexSource = loadFile(vertexSourcePath);
    const String& fragmentSource = loadFile(fragmentSourcePath);

    *m_OpenGL_ID = glCreateProgram();
    unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    loadSourceToShader(vertexShaderID, vertexSource.c_str(), static_cast<int>(vertexSource.length()));
    loadSourceToShader(fragmentShaderID, fragmentSource.c_str(), static_cast<int>(fragmentSource.length()));

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    glAttachShader(*m_OpenGL_ID, vertexShaderID);
    glAttachShader(*m_OpenGL_ID, fragmentShaderID);
    glLinkProgram(*m_OpenGL_ID);

    const ShaderCompilationErrorInfo& info = compilationSuccessful(*m_OpenGL_ID, vertexShaderID, fragmentShaderID);
    if (!info.compilationSuccessful)
        SONIC_LOG_ERROR("Error compiling shader: ", info.infoLog);
}

void Shader::Bind()
{
    glUseProgram(*m_OpenGL_ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::UniformInt(const String& name, int value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform1i(location, value);
}

void Shader::UniformIntArray(const String& name, const int* values, int count)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::UniformFloat(const String& name, float value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform1f(location, value);
}

void Shader::UniformFloat2(const String& name, const glm::vec2& value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void Shader::UniformFloat3(const String& name, const glm::vec3& value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::UniformFloat4(const String& name, const glm::vec4& value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::UniformFloat4(const String& name, const Color& value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniform4f(location, value.r, value.g, value.b, value.a);
}

void Shader::UniformMat4(const String& name, const glm::mat4& value)
{
    int location = glGetUniformLocation(*m_OpenGL_ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
