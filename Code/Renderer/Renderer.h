#pragma once

#include "Math/Transform.h"
#include "Renderer/Camera.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Window.h"

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

    static void Begin(const Shader &shader, const Camera &camera);
    static void End();

    static void Submit(const Mesh &mesh, const Texture &texture, const Transform &transform);

    static Window *GetMainWindow() { return s_MainWindow; }
    static void SetMainWindow(Window &window) { s_MainWindow = &window; }

private:
    static inline Window *s_MainWindow = nullptr;

    static inline const Shader *s_ActiveShader = nullptr;
    static inline const Camera *s_ActiveCamera = nullptr;
};
