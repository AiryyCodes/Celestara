#pragma once

#include "Memory.h"
#include "UI/UIManager.h"
#include "World/World.h"

class Game
{
public:
    Game();

    void Init();
    void Update(float delta);
    void Render();

    UIManager &GetUIManager() { return m_UIManager; }
    const UIManager &GetUIManager() const { return m_UIManager; }

    Ref<World> GetWorld() { return m_World; }

    static Game &Get() { return *s_Instance; }

private:
    static inline Game *s_Instance;

    UIManager m_UIManager;

    Ref<World> m_World;
};
