#pragma once

#include "Math/Math.h"
#include "Memory.h"

class Tile
{
public:
    void SetTextureLayer(int layer) { m_TextureLayer = layer; }
    int GetTextureLayer() const { return m_TextureLayer; }

private:
    // Invalid by default
    int m_TextureLayer = -1;
};

class TileState
{
public:
    TileState(const Ref<Tile> &tile, Vector2i position)
        : m_Tile(tile), m_Position(position) {}

    int GetTextureLayer() const { return m_Tile->GetTextureLayer(); }

    Vector2i GetPosition() const { return m_Position; }

private:
    const Ref<Tile> m_Tile;

    Vector2i m_Position;
};
