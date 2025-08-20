#pragma once

#include "Memory.h"
#include "World/World.h"

class Game
{
public:
    Game();

    void Init();
    void Update(float delta);
    void Render();

    Ref<World> GetWorld() { return m_World; }

    static Game &Get() { return *s_Instance; }

private:
    static inline Game *s_Instance;

    Ref<World> m_World;
};
