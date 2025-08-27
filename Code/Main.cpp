#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

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

    Game game;
    game.Init();

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
        glViewport(0, 0, window.GetFramebufferSize().x, window.GetFramebufferSize().y);

        game.Update(delta);

        Renderer::Begin(Renderer::GetMainShader());

        game.Render();

        Renderer::End();

        window.SwapBuffers();
    }

    Renderer::Shutdown();

    return EXIT_SUCCESS;
}
