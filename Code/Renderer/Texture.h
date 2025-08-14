#pragma once

#include <string>

class Texture
{
public:
    Texture();
    Texture(const std::string &path);

    void Bind() const;
    static void Unbind();

    void SetTexture(const std::string &path);

private:
    void Init();

private:
    unsigned int m_Id = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_NumChannels = 0;

    std::string m_Path;
};
