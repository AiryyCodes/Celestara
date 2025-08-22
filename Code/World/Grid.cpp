#include "World/Grid.h"
#include "Game.h"
#include "Logger.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Physics/Category.h"
#include "Registry/TileRegistry.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vertex.h"
#include "Util/Direction.h"
#include "World/Tile.h"
#include "World/World.h"

#include <box2d/box2d.h>
#include <box2d/types.h>
#include <utility>
#include <vector>

void Grid::Init()
{
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            Vector2i position(x, y);

            Ref<Tile> tile = TileRegistry::GetTile("metal");

            TileState state(tile, position);
            CreateTileCollider(position, &state);

            m_Tiles.emplace(std::make_pair(position, state));
        }
    }

    for (const auto &[pos, tile] : m_Tiles)
    {
        for (Direction direction : DIRECTIONS)
        {
            Vector2i relative = GetRelativeVector(pos, direction);

            const TileState *state = GetState(relative.x, relative.y);

            // If tile is outside of boundaries/not placed, create a collider
            if (state == nullptr)
            {
                CreateEdgeCollider(relative, direction);
            }
        }
    }

    BuildMesh();
}

void Grid::Render()
{
    Renderer::Begin(Renderer::GetMainShader());
    Renderer::Submit(m_Mesh, GetTransform());
}

void Grid::BuildMesh()
{
    std::vector<Vertex> vertices(m_Tiles.size() * 6);
    for (const auto &[position, tile] : m_Tiles)
    {
        for (const auto &vertex : QUAD_VERTICES)
        {
            Vertex newVertex;
            newVertex.Position = vertex.Position + Vector2(position.x, position.y);
            newVertex.UV = vertex.UV;
            newVertex.Layer = tile.GetTextureLayer();

            vertices.emplace_back(newVertex);
        }
    }

    m_Mesh.SetTexture(TileRegistry::GetTextures());
    m_Mesh.SetVertices(vertices);
}

void Grid::CreateTileCollider(const Vector2i &position, const TileState *tile)
{
    Ref<World> world = Game::Get().GetWorld();
    if (!world)
        return;

    // Air tiles should have collisions
    if (tile && !tile->IsAir())
        return;

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = (b2Vec2){(float)position.x, (float)position.y};

    b2BodyId bodyId = b2CreateBody(world->GetPhysicsId(), &bodyDef);
    b2Polygon box = b2MakeBox(0.5f, 0.5f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = PhysicsCategory::Boundary;
    shapeDef.filter.maskBits = PhysicsCategory::Player;
    shapeDef.enablePreSolveEvents = true;

    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    m_Bodies[position] = bodyId;
}

void Grid::CreateEdgeCollider(const Vector2i &position, Direction direction)
{
    Ref<World> world = Game::Get().GetWorld();
    if (!world)
        return;

    b2BodyDef bodyDef = b2DefaultBodyDef();

    Vector2f newPosition = position;
    float halfWidth = 0.5f;
    float halfHeight = 0.5f;

    const float size = 0.001f;

    switch (direction)
    {
    case Direction::North:
        halfHeight = size;
        newPosition.y -= 0.5f + halfHeight;
        break;
    case Direction::South:
        halfHeight = size;
        newPosition.y += 0.5f + halfHeight;
        break;
    case Direction::West:
        halfWidth = size;
        newPosition.x += 0.5f + halfWidth;
        break;
    case Direction::East:
        halfWidth = size;
        newPosition.x -= 0.5f + halfWidth;
        break;
    default:
        halfWidth = 0.5f;
        halfHeight = 0.5f;
    }
    bodyDef.position = (b2Vec2){(float)newPosition.x, (float)newPosition.y};

    b2BodyId bodyId = b2CreateBody(world->GetPhysicsId(), &bodyDef);

    b2Polygon box = b2MakeBox(halfWidth, halfHeight);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.filter.categoryBits = PhysicsCategory::Boundary;
    shapeDef.filter.maskBits = PhysicsCategory::Player;
    shapeDef.enablePreSolveEvents = true;

    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    m_Bodies[position] = bodyId;
}

const TileState *Grid::GetState(int x, int y)
{
    auto it = m_Tiles.find(Vector2i(x, y));
    return it != m_Tiles.end() ? &it->second : nullptr;
}
