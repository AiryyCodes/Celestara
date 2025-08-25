#pragma once

#include "Math/Math.h"
#include "Memory.h"
#include "Renderer/Camera.h"
#include "Renderer/Sprite.h"
#include "Util/Direction.h"
#include "World/Grid.h"
#include "World/WorldObject.h"

#include <box2d/collision.h>
#include <box2d/id.h>

class Player : public WorldObject
{
public:
    Player();

    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void Render() override;

    bool IsJetpackEnabled() const { return m_JetpackEnabled; }
    void SetJetpackEnabled(bool enabled) { m_JetpackEnabled = enabled; }

    b2ShapeId GetShapeId() const { return m_ShapeId; }

private:
    void UpdateAnims(float delta);
    void Move(float delta);

    Ref<Grid> GetGridAtPos(const Vector2f &pos);

    bool IsOverlapping(Vector4 a, Vector4 b);

private:
    Vector2f m_Velocity = Vector2(0.0f, 0.0f);

    float m_Speed = 1.0f;
    float m_JetpackSpeed = 2.0f;

    float m_Acceleration = 1.5f;
    float m_Damping = 5.0f;

    Direction m_Direction;

    bool m_JetpackEnabled = false;

    Ref<Grid> m_Grid;

    Camera m_Camera;

    AnimatedSprite m_IdleSprite;
    AnimatedSprite m_WalkSprite;
    AnimatedSprite m_JetpackSprite;

    b2BodyId m_BodyId;
    b2ShapeId m_ShapeId;
};

static float PlayerRaycastCallaback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context);
