#pragma once

#include "Math/Math.h"
#include <freetype2/ft2build.h>
#include <map>
#include <string>
#include <unordered_map>
#include FT_FREETYPE_H

enum class Font
{
    Main,
};

struct Character
{
    unsigned int TextureID;
    Vector2i Size;    // Size of glyph
    Vector2i Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance;
};

class FontFace
{
public:
    FontFace(const FT_Face &face);

    const FT_Face &GetFace() const { return m_Face; }

    const std::map<char, Character> &GetCharacters() const { return m_Characters; }
    std::map<char, Character> GetCharacters() { return m_Characters; }

private:
    void LoadCharacters();

private:
    FT_Face m_Face;

    std::map<char, Character> m_Characters;
};

class FontManager
{
public:
    static void Init();

    static FontFace *LoadFont(Font font, const std::string &path);

    static FontFace *GetFont(Font font);

    static int GetTextWidth(Font font, const std::string &text, float scale);

private:
    static inline std::unordered_map<Font, FontFace> s_LoadedFonts;

    static inline FT_Library s_Library;
};
