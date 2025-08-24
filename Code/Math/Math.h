#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>

using Vector2 = glm::vec2;
using Vector2f = glm::fvec2;
using Vector2i = glm::ivec2;

using Vector3 = glm::vec3;

using Matrix4 = glm::mat4;

template <typename T>
T Lerp(const T &a, const T &b, float t)
{
    return a * (1.0f - t) + b * t;
}
