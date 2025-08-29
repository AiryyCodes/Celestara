#pragma once

#include "Math/Math.h"

#include <string>

class Shader
{
public:
    Shader();
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();

    void Init();
    void Init(const std::string &vertexPath, const std::string &fragmentPath);
    void Destroy();

    void Bind() const;
    static void Unbind();

    void SetUniform(const std::string &name, const Matrix4 &value) const;
    void SetUniform(const std::string &name, const Vector2 &value) const;
    void SetUniform(const std::string &name, const Vector3 &value) const;
    void SetUniform(const std::string &name, bool value) const;
    void SetUniform(const std::string &name, int value) const;
    void SetUniform(const std::string &name, float value) const;

private:
    unsigned int CompileShader(const std::string &path, const std::string &name, unsigned int type);

private:
    unsigned int m_Id = 0;

    std::string m_VertexPath;
    std::string m_FragmentPath;
};
