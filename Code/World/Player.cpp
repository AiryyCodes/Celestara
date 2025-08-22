#include "World/Player.h"
#include "Game.h"
#include "Input.h"
#include "Logger.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Physics/Category.h"
#include "Renderer/Renderer.h"
#include "World/World.h"

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <glm/geometric.hpp>

Player::Player()
    : m_IdleSprite("Assets/Textures/Player/Idle.png", 16, 16, 4, 0.15f),
      m_WalkSprite("Assets/Textures/Player/Walk.png", 16, 16, 8, 0.1f)
{
    Ref<World> world = Game::Get().GetWorld();

    // Register collision filters/solver callbacks
    // b2World_SetCustomFilterCallback(world->GetPhysicsId(), JetpackCollisionFilter, this);
    b2World_SetPreSolveCallback(world->GetPhysicsId(), JetpackCollisionSolver, this);
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

    float radius = 0.45f;

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
    m_IdleSprite.Update(delta);
    m_WalkSprite.Update(delta);

    if (Input::IsKeyJustDown(GLFW_KEY_X))
    {
        m_JetpackEnabled = !m_JetpackEnabled;

        b2Filter filter = b2Shape_GetFilter(m_ShapeId);
        filter.maskBits = m_JetpackEnabled ? 0 : PhysicsCategory::Boundary;

        b2Shape_SetFilter(m_ShapeId, filter);
    }

    // LOG_INFO("Jetpack: {}", m_JetpackEnabled);

    m_Velocity = Vector2(0.0f, 0.0f);

    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());

    if (Input::IsKeyDown(GLFW_KEY_W))
    {
        m_Velocity.y += 1.0f;
        m_Direction = Direction::North;
    }
    if (Input::IsKeyDown(GLFW_KEY_S))
    {
        m_Velocity.y -= 1.0f;
        m_Direction = Direction::South;
    }
    if (Input::IsKeyDown(GLFW_KEY_A))
    {
        m_Velocity.x -= 1.0f;
        m_Direction = Direction::West;
    }
    if (Input::IsKeyDown(GLFW_KEY_D))
    {
        m_Velocity.x += 1.0f;
        m_Direction = Direction::East;
    }

    if (glm::length(m_Velocity) < 0.0001f)
    {
        m_Direction = Direction::None;
        b2Body_SetLinearVelocity(m_BodyId, {0.0f, 0.0f});
        return;
    }

    m_Velocity = glm::normalize(m_Velocity) * m_Speed;

    b2Vec2 vel = {m_Velocity.x, m_Velocity.y};
    b2Body_SetLinearVelocity(m_BodyId, vel);

    b2Transform bodyTransform = b2Body_GetTransform(m_BodyId);
    GetTransform().SetPosition(Vector2(
        bodyTransform.p.x,
        bodyTransform.p.y));

    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());
}

void Player::Render()
{
    const auto &animShader = Renderer::GetAnimationShader();
    Renderer::Begin(animShader);

    int rowIndex = 2; // default (idle facing down)

    if (m_Velocity.y > 0.0f)
        rowIndex = 3; // up
    if (m_Velocity.y < 0.0f)
        rowIndex = 2; // down
    if (m_Velocity.x < 0.0f)
        rowIndex = 1; // left
    if (m_Velocity.x > 0.0f)
        rowIndex = 0; // right

    if (m_Direction == Direction::None)
        rowIndex = 2; // force idle down

    animShader.SetUniform("u_RowIndex", rowIndex);

    const bool isMoving = glm::length(m_Velocity) > 0.0f;
    (isMoving ? m_WalkSprite : m_IdleSprite).Render(GetTransform());
}

bool JetpackCollisionFilter(b2ShapeId shapeA, b2ShapeId shapeB, void *context)
{
    Player *player = static_cast<Player *>(context);

    if (player->IsJetpackEnabled())
        return false;

    return true;
}

bool JetpackCollisionSolver(b2ShapeId shapeA, b2ShapeId shapeB, b2Manifold *manifold, void *context)
{
    Player *player = static_cast<Player *>(context);

    void *userA = b2Shape_GetUserData(shapeA);
    void *userB = b2Shape_GetUserData(shapeB);

    if (userA == player || userB == player)
    {
        if (player->IsJetpackEnabled())
        {
            return false; // disable collision for player
        }
    }

    return true;
}
