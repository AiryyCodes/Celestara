#include "Renderer/Texture.h"
#include "Logger.h"

#include <string>
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture()
    : m_Path()
{
    glGenTextures(1, &m_Id);
}

Texture::Texture(const std::string &path)
    : m_Path(path)
{
    glGenTextures(1, &m_Id);

    Init();
}

void Texture::Init()
{
    glBindTexture(GL_TEXTURE_2D, m_Id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int width, height, numChannels;
    unsigned char *data = stbi_load(m_Path.c_str(), &width, &height, &numChannels, 0);
    if (data)
    {
        m_Width = width;
        m_Height = height;
        m_NumChannels = numChannels;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture");
    }
    stbi_image_free(data);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetTexture(const std::string &path)
{
    m_Path = path;

    // Update the texture data with the new texture
    Init();
}
