#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
    static bool IsKeyDown(int key);
    static bool IsKeyJustDown(int key);

    static bool IsButtonDown(int key);
    static bool IsButtonJustDown(int key);

    static double GetMouseX();
    static double GetMouseY();

private:
    friend class Window;

    static inline bool s_KeysDown[GLFW_KEY_LAST] = {false};
    static inline bool s_KeysJustDown[GLFW_KEY_LAST] = {false};

    static inline bool s_ButtonsDown[GLFW_MOUSE_BUTTON_LAST] = {false};
    static inline bool s_ButtonsJustDown[GLFW_MOUSE_BUTTON_LAST] = {false};

    static inline double s_MouseX = 0;
    static inline double s_MouseY = 0;
};
