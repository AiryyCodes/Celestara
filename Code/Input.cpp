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
