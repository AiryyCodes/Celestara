#include "Mesh.h"
#include "Vertex.h"

#include <cstddef>
#include <glad/gl.h>
#include <vector>

Mesh::Mesh()
{
    glGenVertexArrays(1, &m_Id);
    glGenBuffers(1, &m_VBO);
}

Mesh::Mesh(const std::vector<Vertex> &vertices)
    : m_Vertices(vertices), m_NumVertices(vertices.size())
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

    glBindVertexArray(m_Id);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);

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
    m_Vertices = vertices;
    m_NumVertices = m_Vertices.size();

    // Upload the new vertices to the GPU
    InitVertices();
}
