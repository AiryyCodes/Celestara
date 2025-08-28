#include "Mesh.h"
#include "Vertex.h"

#include <cstddef>
#include <glad/gl.h>
#include <vector>

Mesh::Mesh(MeshUsage usage)
    : m_Usage(usage)
{
    glGenVertexArrays(1, &m_Id);
    glGenBuffers(1, &m_VBO);
}

Mesh::Mesh(const std::vector<Vertex> &vertices, MeshUsage usage)
    : m_Vertices(vertices), m_NumVertices(vertices.size()), m_Usage(usage)
{
    glGenVertexArrays(1, &m_Id);
    glGenBuffers(1, &m_VBO);

    Init();
}

void Mesh::Init()
{
    InitVertices();
}

void Mesh::InitVertices()
{
    if (m_Vertices.empty())
    {
        return;
    }

    GLenum usage = (m_Usage == MeshUsage::Dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    glBindVertexArray(m_Id);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vertex) * m_Vertices.size(),
                 &m_Vertices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void *)(offsetof(Vertex, Position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void *)(offsetof(Vertex, UV)));

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *)(offsetof(Vertex, Layer)));
}

void Mesh::Bind() const
{
    glBindVertexArray(m_Id);
}

void Mesh::Unbind()
{
    glBindVertexArray(0);
}

void Mesh::SetVertices(const std::vector<Vertex> &vertices)
{
    glBindVertexArray(m_Id);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    if (m_Usage == MeshUsage::Dynamic)
    {
        if (vertices.size() <= m_NumVertices)
        {
            // Fast path: update existing GPU buffer
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        }
        else
        {
            // Slow path: reallocate GPU buffer
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        }
    }
    else
    {
        // Static mesh: always allocate
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    }

    // Vertex attributes (do this every time in case VAO was unbound)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void *)(offsetof(Vertex, Position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void *)(offsetof(Vertex, UV)));

    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *)(offsetof(Vertex, Layer)));

    // Update local CPU-side data
    m_Vertices = vertices;
    m_NumVertices = (int)vertices.size();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
