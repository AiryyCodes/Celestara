#include "Game.h"
#include "Memory.h"

Game::Game()
{
    s_Instance = this;
}

void Game::Init()
{
    m_World = CreateRef<World>();
    m_World->Init();
}

void Game::Update(float delta)
{
    m_World->Update(delta);
}

void Game::Render()
{
    m_World->Render();
}
