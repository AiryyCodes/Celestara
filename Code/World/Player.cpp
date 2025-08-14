#include "World/Player.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

void Player::Init()
{
    GetTransform().SetPosition(Vector2(0.0f, 0.0f));

    m_Mesh.SetVertices(QUAD_VERTICES);
    m_Texture.SetTexture("Assets/Textures/Grass.png");
}

void Player::Render()
{
    Renderer::Submit(m_Mesh, m_Texture, GetTransform());
}
