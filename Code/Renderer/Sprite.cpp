#include "Renderer/Sprite.h"
#include "Math/Transform.h"
#include "Renderer/Renderer.h"

Sprite::Sprite(const std::string &texPath)
    : m_Texture(texPath)
{
    m_Mesh.SetTexture(m_Texture);
    m_Mesh.SetVertices(QUAD_VERTICES);
}

void Sprite::Update(float delta)
{
}

void Sprite::Render(const Transform &transform)
{
    Renderer::Submit(m_Mesh, transform);
}

AnimatedSprite::AnimatedSprite(const std::string &spritePath, int spriteX, int spriteY, int numFrames, float animSpeed)
    : Sprite(spritePath), m_SpriteSheet(GetTexture(), spriteX, spriteY), m_Animation(numFrames, animSpeed)
{
}

void AnimatedSprite::Update(float delta)
{
    m_Animation.Update(delta);
}

void AnimatedSprite::Render(const Transform &transform)
{
    Renderer::Begin(Renderer::GetAnimationShader());
    m_Animation.Render(m_SpriteSheet);

    Sprite::Render(transform);
}
