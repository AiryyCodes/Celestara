#pragma once

#include "Math/Math.h"
#include "Memory.h"

class Tile
{
public:
    Tile()
        : m_Air(false) {}
    Tile(bool air)
        : m_Air(air) {}

    void SetTextureLayer(int layer) { m_TextureLayer = layer; }
    int GetTextureLayer() const { return m_TextureLayer; }

    bool IsAir() const { return m_Air; }

private:
    // Invalid by default
    int m_TextureLayer = -1;

    bool m_Air;
};

class TileState
{
public:
    TileState(const Ref<Tile> &tile, Vector2i position)
        : m_Tile(tile), m_Position(position) {}

    int GetTextureLayer() const { return m_Tile->GetTextureLayer(); }
    bool IsAir() const { return m_Tile->IsAir(); }

    Vector2i GetPosition() const { return m_Position; }

private:
    const Ref<Tile> m_Tile;

    Vector2i m_Position;
};
