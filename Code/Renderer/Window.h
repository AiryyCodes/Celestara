#pragma once

#include "Math/Math.h"

#include <string>

struct GLFWwindow;

class Window
{
public:
    Window(const std::string &title, int width, int height)
        : m_Title(title), m_Width(width), m_Height(height) {}

    void Init();

    void SwapBuffers() const;

    bool IsClosing() const;

    const std::string &GetTitle() const { return m_Title; }
    void SetTitle(const std::string &title);

    int GetWidth() const { return m_Width; }
    void SetWidth(int width);

    int GetHeight() const { return m_Height; }
    void SetHeight(int height);

    Vector2i GetFramebufferSize() const;

    GLFWwindow *GetHandle() const { return m_Handle; }

private:
    std::string m_Title;
    int m_Width;
    int m_Height;

    GLFWwindow *m_Handle;
};
