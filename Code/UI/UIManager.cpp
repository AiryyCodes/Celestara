#include "UI/UIManager.h"
#include "Renderer/Renderer.h"

void UIManager::Init()
{
    m_Mesh.SetVertices(UI_VERTICES);
}

void UIManager::Update()
{
}

void UIManager::Render()
{
    if (!m_ActiveElement)
        return;

    Renderer::Begin(Renderer::GetUIShader());
    m_Mesh.SetTexture(m_ActiveElement->GetTexture());
    Renderer::SubmitUI(m_Mesh, m_ActiveElement->GetPosition(), m_ActiveElement->GetSize(), m_ActiveElement->GetScale());

    m_ActiveElement->Render();
}
