#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Vertex.h"
#include "Renderer/Window.h"

#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

static std::vector<Vertex> QUAD_VERTICES = {
    {{-0.5f, 0.5f}},
    {{-0.5f, -0.5f}},
    {{0.5f, -0.5f}},

    {{-0.5f, 0.5f}},
    {{0.5f, 0.5f}},
    {{0.5f, -0.5f}},
};

int main()
{
    if (!Renderer::PreInit())
    {
        printf("Failed to pre-initialize renderer\n");
        return EXIT_FAILURE;
    }

    Window window("Astralis", 1280, 720);
    window.Init();

    glfwMakeContextCurrent(window.GetHandle());

    if (!Renderer::Init())
    {
        printf("Failed to initialize renderer\n");
        return EXIT_FAILURE;
    }

    Shader shader("Assets/Shaders/Main.vert", "Assets/Shaders/Main.frag");
    shader.Init();

    Mesh mesh(QUAD_VERTICES);

    while (!window.IsClosing())
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        glViewport(0, 0, window.GetWidth(), window.GetHeight());

        Renderer::Begin(shader);

        Renderer::Submit(mesh);

        Renderer::End();

        window.SwapBuffers();
    }

    return EXIT_SUCCESS;
}
