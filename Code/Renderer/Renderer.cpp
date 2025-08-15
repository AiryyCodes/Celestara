#include "Renderer.h"
#include "Math/Math.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>

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

void Renderer::Begin(const Shader &shader, const Camera &camera)
{
    s_ActiveShader = &shader;
    s_ActiveShader->Bind();

    s_ActiveCamera = &camera;
}

void Renderer::Submit(const Mesh &mesh, const Texture &texture, const Transform &transform)
{
    if (!s_ActiveShader || !s_ActiveCamera)
        return;

    Vector2 position = transform.GetPosition();

    Matrix4 matrix(1.0f);
    matrix = glm::translate(matrix, Vector3(position.x, position.y, 0.0f));

    s_ActiveShader->SetUniform("u_Model", matrix);
    s_ActiveShader->SetUniform("u_View", s_ActiveCamera->GetViewMatrix());
    s_ActiveShader->SetUniform("u_Projection", s_ActiveCamera->GetProjectionMatrix());

    texture.Bind();
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.GetNumVertices());
}

void Renderer::End()
{
    if (!s_ActiveShader)
        return;

    s_ActiveShader->Unbind();
}
