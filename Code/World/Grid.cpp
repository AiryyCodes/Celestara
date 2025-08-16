#include "World/Grid.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vertex.h"
#include "World/Tile.h"
#include <utility>
#include <vector>

void Grid::Init()
{
    m_Texture.SetTexture("Assets/Textures/Grass.png");

    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            Vector2i position(x, y);

            Tile tile(position);
            m_Tiles.emplace(std::make_pair(position, tile));
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

            vertices.push_back(newVertex);
        }
    }

    m_Mesh.SetTexture(m_Texture);
    m_Mesh.SetVertices(vertices);
}
