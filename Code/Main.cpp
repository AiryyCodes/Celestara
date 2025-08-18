#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"
#include "World/Grid.h"
#include "World/Player.h"

int main()
{
    if (!Renderer::PreInit())
    {
        printf("Failed to pre-initialize renderer\n");
        return EXIT_FAILURE;
    }

    Window window("Celestara", 1280, 720);
    window.Init();

    Renderer::SetMainWindow(window);

    glfwMakeContextCurrent(window.GetHandle());

    if (!Renderer::Init())
    {
        printf("Failed to initialize renderer\n");
        return EXIT_FAILURE;
    }

    Player player;
    player.Init();

    Grid grid;
    grid.Init();

    float now = 0.0f;
    float last = 0.0f;

    while (!window.IsClosing())
    {
        now = glfwGetTime();
        float delta = now - last;
        last = now;

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        glViewport(0, 0, window.GetWidth(), window.GetHeight());

        player.Update(delta);

        Renderer::Begin(Renderer::GetMainShader());

        grid.Render();
        player.Render();

        Renderer::End();

        window.SwapBuffers();
    }

    Renderer::Shutdown();

    return EXIT_SUCCESS;
}
