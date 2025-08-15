#pragma once

#include "Renderer/Texture.h"
#include "Vertex.h"

#include <vector>

class Mesh
{
public:
    Mesh();
    Mesh(const std::vector<Vertex> &vertices);

    void Bind() const;
    static void Unbind();

    void SetVertices(const std::vector<Vertex> &vertices);
    int GetNumVertices() const { return m_NumVertices; }

    void SetTexture(const Texture &texture) { m_Texture = &texture; }
    const Texture *GetTexture() const { return m_Texture; }

private:
    void Init();
    void InitVertices();

private:
    unsigned int m_Id = 0;
    unsigned int m_VBO = 0;

    std::vector<Vertex> m_Vertices;
    int m_NumVertices = 0;

    const Texture *m_Texture = nullptr;
};
