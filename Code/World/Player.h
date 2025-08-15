#pragma once

#include "Graphics/Animation.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "Util/Direction.h"
#include "World/WorldObject.h"

class Player : public WorldObject
{
public:
    Player();

    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void Render() override;

private:
    float m_Speed = 2.5f;
    Vector2 m_Velocity = Vector2(0.0f, 0.0f);

    Direction m_Direction;

    Camera m_Camera;

    Animation m_IdleAnimation;
    Ref<Spritesheet> m_IdleSheet;
    Texture m_IdleSheetTexture;

    Animation m_WalkAnimation;
    Ref<Spritesheet> m_WalkSheet;
    Texture m_WalkSheetTexture;

    Mesh m_Mesh;
};
