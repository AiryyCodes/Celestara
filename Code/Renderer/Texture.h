#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string &path);

    void Bind() const;
    static void Unbind();

private:
    void Init();

private:
    unsigned int m_Id = 0;

    int m_Width = 0;
    int m_Height = 0;
    int m_NumChannels = 0;

    std::string m_Path;
};
