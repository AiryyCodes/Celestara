#include "UI/UIManager.h"
#include "Input.h"
#include "Logger.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <cmath>

void UIManager::Init()
{
    m_Mesh.SetVertices(UI_VERTICES);
}

void UIManager::Update()
{
    if (!m_ActiveElement)
        return;

    int mouseX = std::floor(static_cast<int>(Input::GetMouseX()));
    int mouseY = std::floor(static_cast<int>(Input::GetMouseY()));

    bool inside = m_ActiveElement->IsInside(mouseX, mouseY);

    if (inside)
    {
        if (Input::IsButtonJustDown(GLFW_MOUSE_BUTTON_LEFT))
        {
            m_ActiveElement->OnClick(mouseX, mouseY, GLFW_MOUSE_BUTTON_LEFT);
        }
        else if (Input::IsButtonJustDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            m_ActiveElement->OnClick(mouseX, mouseY, GLFW_MOUSE_BUTTON_RIGHT);
        }
    }
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
