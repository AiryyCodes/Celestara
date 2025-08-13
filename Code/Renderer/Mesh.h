#pragma once

#include "Vertex.h"

#include <vector>

class Mesh
{
public:
    Mesh();
    Mesh(const std::vector<Vertex> &vertices);

    void Bind() const;
    static void Unbind();

    void SetVertices(const std::vector<Vertex> vertices);

    int GetNumVertices() const { return m_NumVertices; }

private:
    void Init();
    void InitVertices();

private:
    unsigned int m_Id = 0;
    unsigned int m_VBO = 0;

    int m_NumVertices = 0;

    std::vector<Vertex> m_Vertices;
};
