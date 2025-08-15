#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
    static bool IsKeyDown(int key);
    static bool IsKeyJustDown(int key);

private:
    friend class Window;

    static inline bool s_KeysDown[GLFW_KEY_LAST] = {false};
    static inline bool s_KeysJustDown[GLFW_KEY_LAST] = {false};
};
