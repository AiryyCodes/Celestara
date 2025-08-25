#include "Input.h"

bool Input::IsKeyDown(int key)
{
    return s_KeysDown[key];
}

bool Input::IsKeyJustDown(int key)
{
    bool state = s_KeysJustDown[key];
    s_KeysJustDown[key] = false;
    return state;
}

bool Input::IsButtonDown(int key)
{
    return s_ButtonsDown[key];
}

bool Input::IsButtonJustDown(int key)
{
    bool state = s_ButtonsJustDown[key];
    s_ButtonsJustDown[key] = false;
    return state;
}

double Input::GetMouseX()
{
    return s_MouseX;
}

double Input::GetMouseY()
{
    return s_MouseY;
}
