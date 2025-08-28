#include "Window.h"
#include "Game.h"
#include "Input.h"
#include "Math/Math.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Window::Init()
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, true);

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Handle)
    {
        printf("Failed to initialize window\n");
        return;
    }

    glfwSwapInterval(0);

    glfwSetWindowUserPointer(m_Handle, this);

    glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow *handle, int width, int height)
                              {
                                  Window *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));
                                  window->m_Width = width;
                                  window->m_Height = height;

                                  const auto &uiManager = Game::Get().GetUIManager();
                                  if (uiManager.GetActiveElement())
                                    uiManager.GetActiveElement()->OnWindowResize(width, height); });

    glfwSetKeyCallback(m_Handle, [](GLFWwindow *handle, int key, int scancode, int action, int mods)
                       { Input::s_KeysDown[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
                       Input::s_KeysJustDown[key] = action == GLFW_PRESS; });

    glfwSetCursorPosCallback(m_Handle, [](GLFWwindow *handle, double xPos, double yPos)
                             { Input::s_MouseX = xPos;
                             Input::s_MouseY = yPos; });

    glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow *handle, int button, int action, int mods)
                               {
                               if (action == GLFW_PRESS && !Input::s_ButtonsDown[button])
                                    Input::s_ButtonsJustDown[button] = true;
                                Input::s_ButtonsDown[button] = (action != GLFW_RELEASE);
                               
                                Input::s_ButtonsJustDown[button] = action == GLFW_PRESS; });
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_Handle);
}

bool Window::IsClosing() const
{
    return glfwWindowShouldClose(m_Handle);
}

void Window::SetTitle(const std::string &title)
{
    m_Title = title;
    glfwSetWindowTitle(m_Handle, title.c_str());
}

void Window::SetWidth(int width)
{
    m_Width = width;
    glfwSetWindowSize(m_Handle, m_Width, m_Height);
}

void Window::SetHeight(int height)
{
    m_Height = height;
    glfwSetWindowSize(m_Handle, m_Width, m_Height);
}

Vector2i Window::GetFramebufferSize() const
{
    int width;
    int height;
    glfwGetFramebufferSize(m_Handle, &width, &height);

    return Vector2i(width, height);
}
