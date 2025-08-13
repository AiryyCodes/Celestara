#pragma once

#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"

class Renderer
{
public:
    static bool PreInit();
    static bool Init();

    static void Begin(const Shader &shader);

    static void Submit(const Mesh &mesh);

    static void End();

private:
    static inline const Shader *s_ActiveShader;
};
