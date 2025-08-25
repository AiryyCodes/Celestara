#pragma once

#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

using Vector2 = glm::vec2;
using Vector2f = glm::fvec2;
using Vector2i = glm::ivec2;

using Vector3 = glm::vec3;

using Vector4 = glm::vec4;
using Vector4i = glm::ivec4;

using Matrix4 = glm::mat4;

template <typename T>
static inline T Lerp(const T &a, const T &b, float t)
{
    return a * (1.0f - t) + b * t;
}

template <typename T>
static inline T Damp(const T &current, const T &target, float damping, float delta)
{
    damping = std::max(damping, 0.0f);
    delta = std::max(delta, 0.0f);

    float factor = 1.0f - std::exp(-damping * delta);
    return current + (target - current) * factor;
}

static inline Vector2f ScreenToWorld(const Vector2f &mousePos, const Matrix4 &view, const Matrix4 &proj, const Vector4i &viewport)
{
    float winX = mousePos.x;
    float winY = viewport.w - mousePos.y;

    Vector3 nearPoint = glm::unProject(Vector3(winX, winY, 0.0f), view, proj, viewport);
    Vector3 farPoint = glm::unProject(Vector3(winX, winY, 1.0f), view, proj, viewport);

    Vector3 dir = farPoint - nearPoint;

    float t = -nearPoint.z / dir.z;
    Vector3 worldPos = nearPoint + t * dir;

    return Vector2f(worldPos.x, worldPos.y);
}
