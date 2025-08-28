#include "Game.h"
#include "Registry/ItemRegistry.h"
#include "Memory.h"
#include "Registry/TileRegistry.h"
#include "Renderer/Font.h"

Game::Game()
{
    s_Instance = this;
}

void Game::Init()
{
    FontManager::Init();
    FontManager::LoadFont(Font::Main, "Assets/Fonts/PixelifySans-Regular.ttf");

    TileRegistry::Init();
    ItemRegistry::Init();

    m_UIManager.Init();

    m_World = CreateRef<World>();
    m_World->Init();
}

void Game::Update(float delta)
{
    m_World->Update(delta);

    m_UIManager.Update();
}

void Game::Render()
{
    m_World->Render();

    m_UIManager.Render();
}
