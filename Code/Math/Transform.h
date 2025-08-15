#pragma once

#include "Math/Math.h"

class Transform
{
public:
    void Translate(float x, float y);
    void Translate(Vector2 translation);

    const Vector2 &GetPosition() const { return m_Position; }
    void SetPosition(const Vector2 &position) { m_Position = position; }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation) { m_Rotation = rotation; }

private:
    Vector2 m_Position = Vector2(0.0f, 0.0f);
    float m_Rotation = 0.0f;
};
