#pragma once

#include "Math/Math.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "World/Tile.h"
#include "World/WorldObject.h"

#include <unordered_map>

class Grid : WorldObject
{
public:
    Grid();

    void Init() override;
    void Render() override;

private:
    void BuildMesh();

private:
    std::unordered_map<Vector2i, Tile> m_Tiles;

    Texture3D m_Texture;
    Mesh m_Mesh;
};
