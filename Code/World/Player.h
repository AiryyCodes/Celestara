#pragma once

#include "Math/Math.h"
#include "Renderer/Camera.h"
#include "Renderer/Sprite.h"
#include "Util/Direction.h"
#include "World/WorldObject.h"
#include <box2d/id.h>

class Player : public WorldObject
{
public:
    Player();

    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void Render() override;

private:
    float m_Speed = 1.0f;
    Vector2 m_Velocity = Vector2(0.0f, 0.0f);

    Direction m_Direction;

    Camera m_Camera;

    AnimatedSprite m_IdleSprite;
    AnimatedSprite m_WalkSprite;

    b2BodyId m_BodyId;
};
