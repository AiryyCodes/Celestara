#pragma once

#include "Math/Math.h"
#include "Renderer/Mesh.h"
#include "Util/Direction.h"
#include "World/Tile.h"
#include "World/WorldObject.h"

#include <box2d/id.h>
#include <unordered_map>

class Grid : public WorldObject
{
public:
    void Init() override;
    void Render() override;

    const TileState *GetState(int x, int y);

private:
    void BuildMesh();

    void CreateTileCollider(const Vector2i &position, const TileState *tile);
    void CreateEdgeCollider(const Vector2i &position, Direction direction);

private:
    std::unordered_map<Vector2i, TileState> m_Tiles;
    std::unordered_map<Vector2i, b2BodyId> m_Bodies;

    Mesh m_Mesh;
};
