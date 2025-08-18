#include "Renderer/Shader.h"
#include "File.h"
#include "Logger.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
{
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    : m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
{
}

Shader::~Shader()
{
    Destroy();
}

void Shader::Init()
{
}

void Shader::Destroy()
{
    if (m_Id != 0)
    {
        glDeleteProgram(m_Id);
        m_Id = 0;
    }
}

void Shader::Init(const std::string &vertexPath, const std::string &fragmentPath)
{
    m_VertexPath = vertexPath;
    m_FragmentPath = fragmentPath;

    if (m_VertexPath.empty() || m_FragmentPath.empty())
    {
        return;
    }

    unsigned int vertexId = CompileShader(m_VertexPath, "vertex", GL_VERTEX_SHADER);
    if (!vertexId)
    {
        return;
    }

    unsigned int fragmentId = CompileShader(m_FragmentPath, "fragment", GL_FRAGMENT_SHADER);
    if (!fragmentId)
    {
        return;
    }

    m_Id = glCreateProgram();

    glAttachShader(m_Id, vertexId);
    glAttachShader(m_Id, fragmentId);

    glLinkProgram(m_Id);

    int status;

    glGetProgramiv(m_Id, GL_LINK_STATUS, &status);

    if (!status)
    {
        char log[1024];
        glGetProgramInfoLog(m_Id, 1024, NULL, log);

        LOG_ERROR("Failed to compile shader program.\n{}", log);
    }

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

unsigned int Shader::CompileShader(const std::string &path, const std::string &name, unsigned int type)
{
    unsigned int id = glCreateShader(type);

    File file(path);
    std::string source = file.ReadAll();
    if (source.empty())
    {
        LOG_ERROR("Failed to read shader file '{}'", path);
        return 0;
    }

    const char *cSource = source.c_str();
    glShaderSource(id, 1, &cSource, NULL);

    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        int logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

        std::string log(logLength, '\0');
        glGetShaderInfoLog(id, logLength, nullptr, log.data());

        LOG_ERROR("Failed to compile {} shader:\n{}", name, log);
        LOG_ERROR("Shader source:\n{}", source);

        glDeleteShader(id);
        return 0;
    }

    return id;
}

void Shader::Bind() const
{
    if (!m_Id)
    {
        return;
    }

    glUseProgram(m_Id);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::SetUniform(const std::string &name, const Matrix4 &value) const
{
    int location = glGetUniformLocation(m_Id, name.c_str());
    if (location == -1)
    {
        return;
    }

    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string &name, const Vector2 &value) const
{
    int location = glGetUniformLocation(m_Id, name.c_str());
    if (location == -1)
    {
        return;
    }

    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string &name, bool value) const
{
    int location = glGetUniformLocation(m_Id, name.c_str());
    if (location == -1)
    {
        return;
    }

    glUniform1i(location, value);
}

void Shader::SetUniform(const std::string &name, int value) const
{
    int location = glGetUniformLocation(m_Id, name.c_str());
    if (location == -1)
    {
        return;
    }

    glUniform1i(location, value);
}
