#include "Renderer/Camera.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Matrix4 Camera::GetViewMatrix() const
{
    Matrix4 view(1.0f);

    Vector2 position = m_Transform.GetPosition();

    view = glm::translate(view, -Vector3(m_Transform.GetPosition().x, m_Transform.GetPosition().y, 0.0f));
    view = glm::rotate(view, m_Transform.GetRotation(), Vector3(0.0f, 0.0f, 1.0f));

    return view;
}

Matrix4 Camera::GetProjectionMatrix() const
{
    Window *window = Renderer::GetMainWindow();
    float width = window->GetWidth();
    float height = window->GetHeight();

    float scale = m_Zoom * 256;

    float view_width = width / 2.0f;
    float view_height = height / 2.0f;

    Matrix4 proj = glm::ortho(-view_width / scale, view_width / scale, -view_height / scale, view_height / scale);

    return proj;
}
