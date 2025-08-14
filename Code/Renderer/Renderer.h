#pragma once

#include "Math/Transform.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

static std::vector<Vertex> QUAD_VERTICES = {
    {{-0.5f, 0.5f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f}, {1.0f, 0.0f}},

    {{-0.5f, 0.5f}, {0.0f, 1.0f}},
    {{0.5f, 0.5f}, {1.0f, 1.0f}},
    {{0.5f, -0.5f}, {1.0f, 0.0f}},
};

class Renderer
{
public:
    static bool PreInit();
    static bool Init();

    static void Begin(const Shader &shader);

    static void Submit(const Mesh &mesh, const Texture &texture, const Transform &transform);

    static void End();

private:
    static inline const Shader *s_ActiveShader;
};
