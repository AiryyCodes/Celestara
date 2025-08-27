#include "Input.h"

void Input::EndFrame()
{
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
        s_ButtonsJustDown[i] = false;
    for (int i = 0; i < GLFW_KEY_LAST; i++)
        s_KeysJustDown[i] = false;
}

bool Input::IsKeyDown(int key)
{
    return s_KeysDown[key];
}

bool Input::IsKeyJustDown(int key)
{
    return s_KeysJustDown[key];
}

bool Input::IsButtonDown(int button)
{
    return s_ButtonsDown[button];
}

bool Input::IsButtonJustDown(int button)
{
    return s_ButtonsJustDown[button];
}

double Input::GetMouseX()
{
    return s_MouseX;
}

double Input::GetMouseY()
{
    return s_MouseY;
}
