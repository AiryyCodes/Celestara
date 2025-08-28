#pragma once

#include "Renderer/Texture.h"
#include "Vertex.h"

#include <vector>

enum class MeshUsage
{
    Static,
    Dynamic
};

class Mesh
{
public:
    Mesh(MeshUsage usage = MeshUsage::Static);
    Mesh(const std::vector<Vertex> &vertices, MeshUsage usage = MeshUsage::Static);

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

    MeshUsage m_Usage;

    std::vector<Vertex> m_Vertices;
    int m_NumVertices = 0;

    const Texture *m_Texture = nullptr;
};
