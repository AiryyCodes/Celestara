#include "World/Player.h"
#include "Input.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

#include <glm/geometric.hpp>

void Player::Init()
{
    GetTransform().SetPosition(Vector2(0.0f, 0.0f));

    m_Camera.SetZoom(0.25f);

    Renderer::SetActiveCamera(m_Camera);

    m_Mesh.SetVertices(QUAD_VERTICES);
    m_Texture.SetTexture("Assets/Textures/Grass.png");
}

void Player::Update(float delta)
{
    float speed = m_Speed;

    Vector2 velocity = Vector2(0.0f, 0.0f);
    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        velocity.y += speed;
    }
    if (Input::IsKeyDown(GLFW_KEY_S))
    {
        velocity.y -= speed;
    }

    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        velocity.x -= speed;
    }
    if (Input::IsKeyDown(GLFW_KEY_D))
    {
        velocity.x += speed;
    }

    if (glm::length(velocity) > 0.0001f)
    {
        velocity = glm::normalize(velocity);

        m_Camera.GetTransform().Translate(velocity * delta * m_Speed);
    }
}

void Player::Render()
{
    Renderer::Submit(m_Mesh, m_Texture, GetTransform());
}
