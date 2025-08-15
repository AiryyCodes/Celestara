#include "World/Player.h"
#include "Input.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

#include <glm/geometric.hpp>

Player::Player()
    : m_IdleSprite("Assets/Textures/Player/Idle.png", 16, 16, 4, 0.15f),
      m_WalkSprite("Assets/Textures/Player/Walk.png", 16, 16, 8, 0.1f)
{
}

void Player::Init()
{
    GetTransform().SetPosition(Vector2(0.0f, 0.0f));

    m_Camera.SetZoom(0.25f);

    Renderer::SetActiveCamera(m_Camera);
}

void Player::Update(float delta)
{
    m_IdleSprite.Update(delta);
    m_WalkSprite.Update(delta);

    float speed = m_Speed;

    m_Velocity = Vector2(0.0f, 0.0f);
    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        m_Direction = Direction::North;
        m_Velocity.y += 1.0f;
    }
    if (Input::IsKeyDown(GLFW_KEY_S))
    {
        m_Direction = Direction::South;
        m_Velocity.y += -1.0f;
    }

    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        m_Direction = Direction::West;
        m_Velocity.x += -1.0f;
    }
    if (Input::IsKeyDown(GLFW_KEY_D))
    {
        m_Direction = Direction::East;
        m_Velocity.x += 1.0f;
    }

    if (glm::length(m_Velocity) < 0.0001f)
    {
        m_Direction = Direction::None;
        return;
    }

    m_Velocity = glm::normalize(m_Velocity);
    m_Velocity *= m_Speed;
    m_Velocity *= delta;

    GetTransform().Translate(m_Velocity);
    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());
}

void Player::Render()
{
    const auto &animShader = Renderer::GetAnimationShader();
    Renderer::Begin(animShader);

    int rowIndex = 2; // default (idle facing down)

    if (m_Velocity.y > 0.0f)
        rowIndex = 3; // up
    else if (m_Velocity.y < 0.0f)
        rowIndex = 2; // down
    else if (m_Velocity.x < 0.0f)
        rowIndex = 1; // left
    else if (m_Velocity.x > 0.0f)
        rowIndex = 0; // right

    if (m_Direction == Direction::None)
        rowIndex = 2; // force idle down

    animShader.SetUniform("u_RowIndex", rowIndex);

    const bool isMoving = glm::length(m_Velocity) > 0.0f;
    (isMoving ? m_WalkSprite : m_IdleSprite).Render(GetTransform());
}
