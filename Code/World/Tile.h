#pragma once

#include "Math/Math.h"

class Tile
{
public:
    Tile(Vector2i position)
        : m_Position(position) {}

private:
    Vector2i m_Position;
};
