#include "Math/Transform.h"
#include "Math/Math.h"

void Transform::Translate(float x, float y)
{
    Translate(Vector2(x, y));
}

void Transform::Translate(Vector2 translation)
{
    m_Position += translation;
}
