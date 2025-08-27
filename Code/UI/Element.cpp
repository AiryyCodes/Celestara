#include "UI/Element.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

void UIElement::Center()
{
    Window *window = Renderer::GetMainWindow();

    float offsetX = (GetSize().x / 2.0f) * GetScale() - 0.5f;
    float offsetY = (GetSize().y / 2.0f) * GetScale() - 0.5f;

    SetPosition(Vector2f(
        (window->GetWidth()) / 2.0f - offsetX,
        (window->GetHeight()) / 2.0f - offsetY));
}

bool UIElement::IsInside(int mouseX, int mouseY) const
{
    float scaledW = m_Size.x * m_Scale;
    float scaledH = m_Size.y * m_Scale;

    return (mouseX >= m_Position.x && mouseX <= m_Position.x + scaledW &&
            mouseY >= m_Position.y && mouseY <= m_Position.y + scaledH);
}
