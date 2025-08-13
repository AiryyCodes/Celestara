#include "Renderer.h"
#include "Renderer/Texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdio>

bool Renderer::PreInit()
{
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");
        return false;
    }

    return true;
}

bool Renderer::Init()
{
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
    {
        printf("Failed to initialize OpenGL\n");
        return false;
    }

    return true;
}

void Renderer::Begin(const Shader &shader)
{
    s_ActiveShader = &shader;

    s_ActiveShader->Bind();
}

void Renderer::Submit(const Mesh &mesh, const Texture &texture)
{
    texture.Bind();
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.GetNumVertices());
}

void Renderer::End()
{
    s_ActiveShader->Unbind();
}
