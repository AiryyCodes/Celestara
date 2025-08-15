#include "Graphics/Spritesheet.h"

Spritesheet::Spritesheet(const Texture &texture, int tileWidth, int tileHeight)
{
    m_Columns = texture.GetWidth() / tileWidth;
    m_Rows = texture.GetHeight() / tileHeight;
}
