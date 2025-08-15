#include "Math/Math.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"
#include "World/Player.h"

#include <cstdio>
#include <cstdlib>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

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

    Shader shader("Assets/Shaders/Main.vert", "Assets/Shaders/Main.frag");
    shader.Init();

    Player player;
    player.Init();

    Camera camera;
    camera.GetTransform().SetPosition(Vector2(0.0f, 1.0f));

    while (!window.IsClosing())
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);
        glViewport(0, 0, window.GetWidth(), window.GetHeight());

        Renderer::Begin(shader, camera);

        player.Render();

        Renderer::End();

        window.SwapBuffers();
    }

    return EXIT_SUCCESS;
}
