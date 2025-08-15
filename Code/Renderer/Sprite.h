#pragma once

#include "Graphics/Animation.h"
#include "Graphics/Spritesheet.h"
#include "Math/Transform.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Renderer/Vertex.h"

#include <string>
#include <vector>

class Sprite
{
public:
    Sprite(const std::string &texPath);

    virtual void Update(float delta);
    virtual void Render(const Transform &transform);

    void SetVertices(const std::vector<Vertex> &vertices) { m_Mesh.SetVertices(vertices); }
    const Mesh &GetMesh() const { return m_Mesh; }

    const Texture &GetTexture() const { return m_Texture; }
    void SetTexture(const std::string &path)
    {
        m_Texture.SetTexture(path);
        m_Mesh.SetTexture(m_Texture);
    }

private:
    Mesh m_Mesh;
    Texture m_Texture;
};

class AnimatedSprite : public Sprite
{
public:
    AnimatedSprite(const std::string &spritePath, int spriteX, int spriteY, int numFrames, float animSpeed);

    void Update(float delta) override;
    void Render(const Transform &transform) override;

private:
    Animation m_Animation;
    Spritesheet m_SpriteSheet;
};
