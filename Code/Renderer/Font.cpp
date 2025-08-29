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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (uint32_t c = 32; c < 128; c++)
    {
        if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("Failed to load glyph '{}'", c);
            continue;
        }

        int w = m_Face->glyph->bitmap.width;
        int h = m_Face->glyph->bitmap.rows;

        Character character;
        character.Size = glm::ivec2(w, h);
        character.Bearing = glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top);
        character.Advance = m_Face->glyph->advance.x;

        if (w > 0 && h > 0)
        {
            // Only create a texture if the glyph has pixels
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, m_Face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            character.TextureID = texture;
        }
        else
        {
            // No pixels (like space) → TextureID = 0
            character.TextureID = 0;
        }

        m_Characters.insert({static_cast<char>(c), character});
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

int FontManager::GetTextWidth(Font font, const std::string &text, float scale)
{
    const FontFace &face = s_LoadedFonts.at(font);

    int width = 0;
    FT_UInt prev = 0;

    for (char c : text)
    {
        FT_UInt glyph_index = FT_Get_Char_Index(face.GetFace(), c);

        if (prev && glyph_index)
        {
            FT_Vector delta;
            FT_Get_Kerning(face.GetFace(), prev, glyph_index, FT_KERNING_DEFAULT, &delta);
            width += (delta.x >> 6); // add kerning
        }

        if (FT_Load_Glyph(face.GetFace(), glyph_index, FT_LOAD_DEFAULT))
            continue;

        width += (face.GetFace()->glyph->advance.x >> 6); // add glyph advance
        prev = glyph_index;
    }

    return width * scale;
}
