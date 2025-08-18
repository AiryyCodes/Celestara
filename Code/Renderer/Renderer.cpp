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

    s_MainShader.Init("Assets/Shaders/Main.vert", "Assets/Shaders/Main.frag");
    s_AnimationShader.Init("Assets/Shaders/Animation.vert", "Assets/Shaders/Animation.frag");

    return true;
}

void Renderer::Shutdown()
{
    s_MainShader.Destroy();
    s_AnimationShader.Destroy();
}

void Renderer::Begin(const Shader &shader, const Camera &camera)
{
    s_ActiveShader = &shader;
    s_ActiveShader->Bind();

    s_ActiveCamera = &camera;
}

void Renderer::Begin(const Shader &shader)
{
    s_ActiveShader = &shader;
    s_ActiveShader->Bind();
}

void Renderer::Submit(const Mesh &mesh, const Transform &transform)
{
    if (!s_ActiveShader || !s_ActiveCamera)
        return;

    Vector2 position = transform.GetPosition();

    Matrix4 matrix(1.0f);
    matrix = glm::translate(matrix, Vector3(position.x, position.y, 0.0f));

    s_ActiveShader->SetUniform("u_Model", matrix);
    s_ActiveShader->SetUniform("u_View", s_ActiveCamera->GetViewMatrix());
    s_ActiveShader->SetUniform("u_Projection", s_ActiveCamera->GetProjectionMatrix());

    switch (mesh.GetTexture()->GetType())
    {
    case TextureType::Texture2D:
        s_ActiveShader->SetUniform("u_IsSamplerArray", false);
    case TextureType::Texture2DArray:
        s_ActiveShader->SetUniform("u_IsSamplerArray", true);
    }

    mesh.GetTexture()->Bind();
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.GetNumVertices());

    Texture::Unbind();
    Texture3D::Unbind();
}

void Renderer::End()
{
    if (!s_ActiveShader)
        return;

    s_ActiveShader->Unbind();
}
