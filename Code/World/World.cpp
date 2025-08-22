#include "World/World.h"
#include "Logger.h"
#include "World/Grid.h"

#include <box2d/box2d.h>
#include <box2d/types.h>

void World::Init()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};

    m_PhysicsId = b2CreateWorld(&worldDef);

    AddObject<Grid>();
    m_Player = AddObject<Player>();
}

void World::Update(float delta)
{
    const float timeStep = 1.0f / 60.0f;
    const int subStepCount = 4;

    b2World_Step(m_PhysicsId, timeStep, subStepCount);

    for (const auto &object : m_Objects)
    {
        object->Update(delta);
    }
}

void World::Render()
{
    for (const auto &object : m_Objects)
    {
        object->Render();
    }
}
