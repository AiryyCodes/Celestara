#include "Window.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Window::Init()
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, true);

    m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Handle)
    {
        printf("Failed to initialize window\n");
        return;
    }

    glfwSetWindowUserPointer(m_Handle, this);

    glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow *handle, int width, int height)
                              { Window *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));
                              window->m_Width = width;
                              window->m_Height = height; });
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
