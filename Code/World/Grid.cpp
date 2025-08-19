#include "World/Grid.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Registry/TileRegistry.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vertex.h"
#include "World/Tile.h"

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
            if (x == 2 && y == 2)
            {
                tile = TileRegistry::GetTile("air");
            }

            TileState state(tile, position);
            m_Tiles.emplace(std::make_pair(position, state));
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
