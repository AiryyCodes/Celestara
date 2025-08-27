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
