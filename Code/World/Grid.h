#pragma once

#include "Math/Math.h"
#include "Math/Transform.h"
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
    const TileState *GetState(const Vector2i &pos);
    const TileState *GetState(const Transform &transform) const;

    Vector2i WorldToGrid(const Vector2 &pos) const;

private:
    void BuildMesh();

    void CreateTileCollider(const Vector2i &position, const TileState *tile);
    void CreateEdgeCollider(const Vector2i &position, Direction direction);

    Vector2 GridToLocal(const Vector2 &worldPos) const;

private:
    std::unordered_map<Vector2i, TileState> m_Tiles;
    std::unordered_map<Vector2i, b2BodyId> m_Bodies;

    Mesh m_Mesh;
};
