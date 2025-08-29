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

    m_Type = TextureType::Texture2D;
}

Texture::Texture(const std::string &path)
    : m_Path(path)
{
    glGenTextures(1, &m_Id);

    m_Type = TextureType::Texture2D;

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

Texture3D::Texture3D(int maxWidth, int maxHeight)
    : m_MaxWidth(maxWidth), m_MaxHeight(maxHeight), m_Paths()
{
    glGenTextures(1, &m_Id);

    SetType(TextureType::Texture2DArray);
}

Texture3D::Texture3D(int maxWidth, int maxHeight, const std::vector<std::string> &paths)
    : m_MaxWidth(maxWidth), m_MaxHeight(maxHeight), m_Paths(paths)
{
    glGenTextures(1, &m_Id);

    SetType(TextureType::Texture2DArray);

    Init();
}

void Texture3D::Init()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_Id);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int numTiles = m_Paths.size();

    // Allocate storage for the array
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        GL_RGBA8,
        m_MaxWidth,
        m_MaxHeight,
        numTiles,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr);

    for (int i = 0; i < numTiles; ++i)
    {
        int w, h, n;
        unsigned char *data = stbi_load(m_Paths[i].c_str(), &w, &h, &n, 4); // force RGBA
        if (data)
        {
            if (w != m_MaxWidth || h != m_MaxHeight)
            {
                LOG_ERROR("Tile size mismatch for " + m_Paths[i]);
                stbi_image_free(data);
                continue;
            }

            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,       // mip level
                0, 0, i, // x, y, layer
                m_MaxWidth,
                m_MaxHeight,
                1, // depth = 1 layer
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data);

            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("Failed to load texture: " + m_Paths[i]);
        }
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture3D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_Id);
}

void Texture3D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

int Texture3D::AddTexture(const std::string &path)
{
    m_Paths.push_back(path);

    // Update the texture data with the new textures
    Init();

    return m_Paths.size() - 1;
}
