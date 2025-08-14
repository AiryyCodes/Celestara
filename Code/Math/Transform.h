#pragma once

#include "Math/Math.h"

class Transform
{
public:
    const Vector2 &GetPosition() const { return m_Position; }
    void SetPosition(const Vector2 &position) { m_Position = position; }

private:
    Vector2 m_Position = Vector2(0.0f, 0.0f);
};
