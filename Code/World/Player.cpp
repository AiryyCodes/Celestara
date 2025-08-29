#include "World/Player.h"
#include "Game.h"
#include "Input.h"
#include "Inventory/Item.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Physics/Category.h"
#include "Registry/TileRegistry.h"
#include "Renderer/Renderer.h"
#include "UI/Elements/InventoryUI.h"
#include "Util/Direction.h"
#include "Util/String.h"
#include "World/Grid.h"
#include "World/World.h"

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <cmath>
#include <glm/geometric.hpp>
#include <memory>
#include <string>

Player::Player()
    : m_IdleSprite("Assets/Textures/Player/Idle.png", 16, 16, 4, 0.15f),
      m_WalkSprite("Assets/Textures/Player/Walk.png", 16, 16, 8, 0.1f),
      m_JetpackSprite("Assets/Textures/Player/Jetpack.png", 16, 16, 1, 0.0f),
      m_Inventory(4, 4)
{

    ItemStack stack;
    stack.Id = 0;
    stack.Quantity = 1000;

    m_Inventory.SetItem(0, stack);
    // m_Inventory.SetItem(12, stack);

    m_InventoryUI = CreateRef<InventoryUI>(m_Inventory);
    m_InventoryUI->FillSlots(4, 4);
}

void Player::Init()
{
    Ref<World> world = Game::Get().GetWorld();
    UIManager &uiManager = Game::Get().GetUIManager();
    // uiManager.SetElement(m_InventoryUI);

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
    shapeDef.material.friction = 0.0f;
    shapeDef.userData = this;
    shapeDef.filter.categoryBits = PhysicsCategory::Player;
    shapeDef.filter.maskBits = PhysicsCategory::Boundary | PhysicsCategory::Player;
    shapeDef.enablePreSolveEvents = true;

    // Attach the circle to the body
    m_ShapeId = b2CreateCircleShape(m_BodyId, &shapeDef, &circle);
}

void Player::Update(float delta)
{
    Ref<World> world = Game::Get().GetWorld();

    UpdateAnims(delta);

    // toggle jetpack
    if (Input::IsKeyJustDown(GLFW_KEY_X) && m_Grid != nullptr)
    {
        m_JetpackEnabled = !m_JetpackEnabled;
        b2Filter filter = b2Shape_GetFilter(m_ShapeId);
        filter.maskBits = m_JetpackEnabled ? 0 : PhysicsCategory::Boundary | PhysicsCategory::Player;
        b2Shape_SetFilter(m_ShapeId, filter);

        if (!m_JetpackEnabled)
            m_Velocity = {}; // reset once when jetpack is disabled
    }

    Move(delta);

    // Inventory open/close
    if (Input::IsKeyJustDown(GLFW_KEY_E))
    {
        if (Game::Get().GetUIManager().GetActiveElement())
            Game::Get().GetUIManager().SetElement(nullptr);
        else
            Game::Get().GetUIManager().SetElement(m_InventoryUI);
    }

    // Place tiles on grid
    if (Input::IsButtonJustDown(GLFW_MOUSE_BUTTON_LEFT) && !Game::Get().GetUIManager().GetActiveElement())
    {
        Vector2f mousePos = Vector2f((float)Input::GetMouseX(), (float)Input::GetMouseY());
        Vector4i viewport = Vector4i(0, 0, Renderer::GetMainWindow()->GetWidth(), Renderer::GetMainWindow()->GetHeight());

        Vector2f worldPos = ScreenToWorld(mousePos, m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix(), viewport);

        Ref<Grid> grid = GetGridAtPos(worldPos);
        if (!grid)
        {
            return;
        }

        Vector2i gridPos = grid->WorldToGrid(worldPos);

        Vector4 tileAABB(
            gridPos.x, gridPos.y,
            gridPos.x + 1.0f, gridPos.y + 1.0f);

        Vector2f position = GetTransform().GetPosition();

        Vector4 playerAABB(
            position.x, position.y,
            position.x + 1.0f, position.y + 1.0f);

        if (IsOverlapping(playerAABB, tileAABB))
        {
            return;
        }

        Ref<Tile> tile = TileRegistry::GetTile("air");
        Ref<TileState> state = CreateRef<TileState>(tile, gridPos);
        grid->SetState(gridPos, state);
    }

    // Get the grid under the player
    m_Grid = GetGridAtPos(GetTransform().GetPosition());
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

    std::string xCoord = ToString(GetTransform().GetPosition().x, 4);
    std::string yCoord = ToString(GetTransform().GetPosition().y, 4);

    Renderer::SubmitText("X: " + xCoord + " Y: " + yCoord, Font::Main, Vector2f(4.0f, 6.0f), 0.75f, Vector3(1.0f, 1.0f, 1.0f));
}

void Player::UpdateAnims(float delta)
{
    m_IdleSprite.Update(delta);
    m_WalkSprite.Update(delta);
    m_JetpackSprite.Update(delta);
}

void Player::Move(float delta)
{
    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());

    // Get input
    Vector2f input(0.0f);

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
    }

    // Apply damping
    Vector2 targetVel = hasInput ? input * (m_JetpackEnabled ? m_JetpackSpeed : m_Speed) : Vector2(0.0f);

    if (m_JetpackEnabled)
    {
        // Jetpack mode: smooth acceleration
        float dampingFactor = 1.0f - std::exp(-m_Damping * delta);
        m_Velocity = Damp(m_Velocity, targetVel, m_Damping, delta);
    }
    else
    {
        // Walking mode: instant velocity
        m_Velocity = targetVel;
    }

    // Clamp max speed
    float maxSpeed = m_JetpackEnabled ? m_JetpackSpeed : m_Speed;
    float len = glm::length(m_Velocity);

    if (len > maxSpeed)
        m_Velocity = (m_Velocity / len) * maxSpeed;

    float stopThreshold = 7.5f * delta;

    if (len < stopThreshold)
        m_Velocity = {};

    b2Body_SetLinearVelocity(m_BodyId, {m_Velocity.x, m_Velocity.y});

    b2Transform bodyTransform = b2Body_GetTransform(m_BodyId);
    GetTransform().SetPosition(Vector2(bodyTransform.p.x, bodyTransform.p.y));

    m_Camera.GetTransform().SetPosition(GetTransform().GetPosition());
}

Ref<Grid> Player::GetGridAtPos(const Vector2f &pos)
{
    Ref<World> world = Game::Get().GetWorld();

    // Get the grid under the player
    for (auto &obj : world->GetObjects())
    {
        Ref<Grid> grid = std::dynamic_pointer_cast<Grid>(obj);
        if (!grid)
            continue;

        Vector2i gridPos = grid->WorldToGrid(pos);

        const Ref<TileState> tile = grid->GetState(gridPos);
        return tile != nullptr ? grid : nullptr;
    }

    return nullptr;
}

bool Player::IsOverlapping(Vector4 a, Vector4 b)
{
    return a.z > b.x && a.x < b.z && // x-axis overlap
           a.w > b.y && a.y < b.w;   // y-axis overlap
}
