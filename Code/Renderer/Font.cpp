#include "Renderer/Font.h"
#include "Logger.h"

#include <cstdlib>
#include <utility>
#include <glad/gl.h>

FontFace::FontFace(const FT_Face &face)
    : m_Face(face)
{
    LoadCharacters();
}

void FontFace::LoadCharacters()
{
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("Failed to load glyph '{}'", c);
            continue;
        }

        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            m_Face->glyph->bitmap.width,
            m_Face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            m_Face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Store character for later use
        Character character;
        character.TextureID = texture;
        character.Size = glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows);
        character.Bearing = glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top);
        character.Advance = m_Face->glyph->advance.x;

        m_Characters.insert(std::pair<char, Character>(c, character));
    }
}

void FontManager::Init()
{
    if (FT_Init_FreeType(&s_Library))
    {
        LOG_ERROR("Failed to initialize FreeType");
        exit(EXIT_FAILURE);
    }
}

FontFace *FontManager::LoadFont(Font font, const std::string &path)
{
    auto it = s_LoadedFonts.find(font);
    if (it != s_LoadedFonts.end())
        return nullptr;

    FT_Face face;
    if (FT_New_Face(s_Library, path.c_str(), 0, &face))
    {
        LOG_ERROR("Failed to load font '{}'", path);
        return nullptr;
    }

    FT_Set_Pixel_Sizes(face, 0, 36);

    FontFace myFace(face);

    auto ret = s_LoadedFonts.emplace(std::make_pair(font, myFace));

    return &ret.first->second;
}

FontFace *FontManager::GetFont(Font font)
{
    auto it = s_LoadedFonts.find(font);
    if (it == s_LoadedFonts.end())
        return nullptr;

    return &it->second;
}
