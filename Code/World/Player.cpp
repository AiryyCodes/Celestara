#include "World/Player.h"
#include "Input.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

#include <glm/geometric.hpp>

Player::Player()
    : m_IdleAnimation(4, 0.15f), m_WalkAnimation(8, 0.1f)
{
}

void Player::Init()
{
    GetTransform().SetPosition(Vector2(0.0f, 0.0f));

    m_Camera.SetZoom(0.25f);

    Renderer::SetActiveCamera(m_Camera);

    m_IdleSheetTexture.SetTexture("Assets/Textures/Player/Idle.png");
    m_WalkSheetTexture.SetTexture("Assets/Textures/Player/Walk.png");

    int spriteSize = 16;
    m_IdleSheet = CreateRef<Spritesheet>(m_IdleSheetTexture, spriteSize, spriteSize);
    m_WalkSheet = CreateRef<Spritesheet>(m_WalkSheetTexture, spriteSize, spriteSize);

    m_Mesh.SetTexture(m_IdleSheetTexture);
    m_Mesh.SetVertices(QUAD_VERTICES);
}

void Player::Update(float delta)
{
    m_IdleAnimation.Update(delta);
    m_WalkAnimation.Update(delta);

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

    if (m_Velocity.y > 0.0f)
    {
        animShader.SetUniform("u_RowIndex", 3);
    }
    if (m_Velocity.y < 0.0f)
    {
        animShader.SetUniform("u_RowIndex", 2);
    }
    if (m_Velocity.x < 0.0f)
    {
        animShader.SetUniform("u_RowIndex", 1);
    }
    if (m_Velocity.x > 0.0f)
    {
        animShader.SetUniform("u_RowIndex", 0);
    }
    if (m_Direction == Direction::None)
    {
        animShader.SetUniform("u_RowIndex", 2);
    }

    if (glm::length(m_Velocity) > 0.0f)
    {
        m_Mesh.SetTexture(m_WalkSheetTexture);
        m_WalkAnimation.Render(*m_WalkSheet.get());
    }
    else
    {
        m_Mesh.SetTexture(m_IdleSheetTexture);
        m_IdleAnimation.Render(*m_IdleSheet.get());
    }

    Renderer::Submit(m_Mesh, GetTransform());
}
