#pragma once

#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"
#include "World/WorldObject.h"

class Player : public WorldObject
{
public:
    virtual void Init() override;
    virtual void Render() override;

private:
    Mesh m_Mesh;
    Texture m_Texture;
};
