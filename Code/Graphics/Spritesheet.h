#pragma once

#include "Renderer/Texture.h"

class Spritesheet
{
public:
    Spritesheet(const Texture &texture, int tileWidth, int tileHeight);

    int GetColumns() const { return m_Columns; }
    int GetRows() const { return m_Rows; }

private:
    int m_Columns = 0;
    int m_Rows = 0;
};
