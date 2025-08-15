#pragma once

#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "World/WorldObject.h"

class Player : public WorldObject
{
public:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void Render() override;

private:
    float m_Speed = 2.5f;

    Camera m_Camera;

    Mesh m_Mesh;
    Texture m_Texture;
};
