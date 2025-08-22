#pragma once

#include "Memory.h"
#include "World/Player.h"
#include "World/WorldObject.h"

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <utility>
#include <vector>

class World
{
public:
    void Init();
    void Update(float delta);
    void Render();

    template <typename T, typename... Args>
    Ref<T> AddObject(Args &&...args)
    {
        Ref<T> object = CreateRef<T>(std::forward<Args>(args)...);
        object->Init();

        m_Objects.emplace_back(object);

        return object;
    }

    const std::vector<Ref<WorldObject>> &GetObjects() const { return m_Objects; }

    b2WorldId GetPhysicsId() const { return m_PhysicsId; }

    const Ref<Player> &GetPlayer() const { return m_Player; }

private:
    std::vector<Ref<WorldObject>> m_Objects;

    Ref<Player> m_Player;

    b2WorldId m_PhysicsId;
};
