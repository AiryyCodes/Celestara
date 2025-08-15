#pragma once

#include "Math/Math.h"
#include "Math/Transform.h"

class Camera
{
public:
    Transform &GetTransform() { return m_Transform; }
    const Transform &GetTransform() const { return m_Transform; }

    Matrix4 GetViewMatrix() const;
    Matrix4 GetProjectionMatrix() const;

    float GetZoom() const { return m_Zoom; }
    void SetZoom(float zoom) { m_Zoom = zoom; }

private:
    float m_Zoom = 1.0f;

    Transform m_Transform;
};
