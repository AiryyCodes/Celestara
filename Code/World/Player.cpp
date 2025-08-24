#include "World/Player.h"
#include "Game.h"
#include "Input.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Physics/Category.h"
#include "Renderer/Renderer.h"
#include "Util/Direction.h"
#include "World/Grid.h"
#include "World/World.h"

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <cmath>
#include <glm/geometric.hpp>
#include <memory>

Player::Player()
    : m_IdleSprite("Assets/Textures/Player/Idle.png", 16, 16, 4, 0.15f),
      m_WalkSprite("Assets/Textures/Player/Walk.png", 16, 16, 8, 0.1f),
      m_JetpackSprite("Assets/Textures/Player/Jetpack.png", 16, 16, 1, 0.0f)
{
}

void Player::Init()
{
    Ref<World> world = Game::Get().GetWorld();

    GetTransform().SetPosition(Vector2(2.0f, 2.0f));

    m_Camera.SetZoom(0.25f);

    Renderer::SetActiveCamera(m_Camera);

    Vector2 posPixels = GetTransform().GetPosition();

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){posPixels.x,
                                posPixels.y};

    m_BodyId = b2CreateBody(world->GetPhysicsId(), &bodyDef);

    float radius = 0.5f;

    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = radius;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    shapeDef.userData = this;
    shapeDef.filter.categoryBits = PhysicsCategory::Player;
    shapeDef.filter.maskBits = PhysicsCategory::Boundary;
    shapeDef.enablePreSolveEvents = true;

    // Attach the circle to the body
    m_ShapeId = b2CreateCircleShape(m_BodyId, &shapeDef, &circle);
}

void Player::Update(float delta)
{
    Ref<World> world = Game::Get().GetWorld();

    // Get the grid under the player
    for (auto &obj : world->GetObjects())
    {
        Ref<Grid> grid = std::dynamic_pointer_cast<Grid>(obj);
        if (!grid)
            continue;

        Vector2i gridPos = grid->WorldToGrid(GetTransform().GetPosition());

        const TileState *tile = grid->GetState(GetTransform());
        if (tile)
        {
            m_Grid = grid;
            break;
        }
        else
        {
            m_Grid = nullptr;
        }
    }

    m_IdleSprite.Update(delta);
    m_WalkSprite.Update(delta);
    m_JetpackSprite.Update(delta);

    // toggle jetpack
    if (Input::IsKeyJustDown(GLFW_KEY_X) && m_Grid != nullptr)
    {
        m_JetpackEnabled = !m_JetpackEnabled;
        b2Filter filter = b2Shape_GetFilter(m_ShapeId);
        filter.maskBits = m_JetpackEnabled ? 0 : PhysicsCategory::Boundary;
        b2Shape_SetFilter(m_ShapeId, filter);

        if (!m_JetpackEnabled)
            m_Velocity = {}; // reset once when jetpack is disabled
    }

    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());

    // Get input
    glm::vec2 input(0.0f);

    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        input.y += 1.0f;
        m_Direction = Direction::North;
    }
    if (Input::IsKeyDown(GLFW_KEY_S))
    {
        input.y -= 1.0f;
        m_Direction = Direction::South;
    }
    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        input.x -= 1.0f;
        m_Direction = Direction::West;
    }
    if (Input::IsKeyDown(GLFW_KEY_D))
    {
        input.x += 1.0f;
        m_Direction = Direction::East;
    }

    bool hasInput = glm::length(input) > 0.0001f;

    if (!hasInput)
        m_Direction = Direction::None;

    if (hasInput)
    {
        input = glm::normalize(input);
        m_Velocity += input * m_Acceleration * delta;

        if (fabs(input.x) > fabs(input.y))
        {
            m_Direction = input.x > 0 ? Direction::East : Direction::West;
        }
        else
        {
            m_Direction = input.y > 0 ? Direction::North : Direction::South;
        }
    }

    // Apply damping
    if (m_JetpackEnabled)
    {
        // Smooth acceleration (jetpack mode)
        glm::vec2 targetVel = input * m_JetpackSpeed;
        m_Velocity = Lerp(m_Velocity, targetVel, m_Damping * delta);
    }
    else
    {
        // Raw velocity (walking mode)
        m_Velocity = input * m_Speed;
    }

    // Clamp max speed
    float maxSpeed = m_JetpackEnabled ? m_JetpackSpeed : m_Speed;
    float len = glm::length(m_Velocity);

    if (len > maxSpeed)
        m_Velocity = (m_Velocity / len) * maxSpeed;

    if (len < 0.1f)
        m_Velocity = {};

    b2Body_SetLinearVelocity(m_BodyId, {m_Velocity.x, m_Velocity.y});

    b2Transform bodyTransform = b2Body_GetTransform(m_BodyId);
    GetTransform().SetPosition(Vector2(bodyTransform.p.x, bodyTransform.p.y));

    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());
}

void Player::Render()
{
    const auto &animShader = Renderer::GetAnimationShader();
    Renderer::Begin(animShader);

    const float deadzone = 0.5f;
    int rowIndex = 2; // default down

    if (glm::length(m_Velocity) > 0.01f)
    {
        // Horizontal priority
        if (m_Velocity.x < -deadzone)
            rowIndex = 1; // left
        else if (m_Velocity.x > deadzone)
            rowIndex = 0; // right
        else
        {
            // Only use vertical if horizontal is near zero
            if (m_Velocity.y > deadzone)
                rowIndex = 3; // up
            else if (m_Velocity.y < -deadzone)
                rowIndex = 2; // down
        }
    }

    animShader.SetUniform("u_RowIndex", rowIndex);

    const bool isMoving = m_Direction != Direction::None && glm::length(m_Velocity) > 0.01f;

    if (m_JetpackEnabled)
    {
        m_JetpackSprite.Render(GetTransform());
    }
    else
    {
        (isMoving ? m_WalkSprite : m_IdleSprite).Render(GetTransform());
    }
}
