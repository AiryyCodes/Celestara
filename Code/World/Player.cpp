#include "World/Player.h"
#include "Input.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

void Player::Init()
{
    GetTransform().SetPosition(Vector2(0.0f, 0.0f));

    Renderer::SetActiveCamera(m_Camera);

    m_Mesh.SetVertices(QUAD_VERTICES);
    m_Texture.SetTexture("Assets/Textures/Grass.png");
}

void Player::Update()
{
    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        m_Camera.GetTransform().Translate(0.0f, m_Speed);
    }
    if (Input::IsKeyDown(GLFW_KEY_S))
    {
        m_Camera.GetTransform().Translate(0.0f, -m_Speed);
    }

    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        m_Camera.GetTransform().Translate(-m_Speed, 0.0f);
    }
    if (Input::IsKeyDown(GLFW_KEY_D))
    {
        m_Camera.GetTransform().Translate(m_Speed, 0.0f);
    }
}

void Player::Render()
{
    Renderer::Submit(m_Mesh, m_Texture, GetTransform());
}
