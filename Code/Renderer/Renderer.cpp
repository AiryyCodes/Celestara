#include "Renderer.h"
#include "Math/Math.h"
#include "Memory.h"
#include "Renderer/Camera.h"
#include "Renderer/Font.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_transform.hpp>
#include <string>
#include <vector>

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    s_MainShader.Init("Assets/Shaders/Main.vert", "Assets/Shaders/Main.frag");
    s_AnimationShader.Init("Assets/Shaders/Animation.vert", "Assets/Shaders/Animation.frag");
    s_UIShader.Init("Assets/Shaders/UI.vert", "Assets/Shaders/UI.frag");
    s_SlotShader.Init("Assets/Shaders/Slot.vert", "Assets/Shaders/Slot.frag");
    s_TextShader.Init("Assets/Shaders/Text.vert", "Assets/Shaders/Text.frag");

    std::vector<Vertex> emptyVerts(6); // one quad
    s_TextMesh = CreateRef<Mesh>(emptyVerts, MeshUsage::Dynamic);

    return true;
}

void Renderer::Shutdown()
{
    s_MainShader.Destroy();
    s_AnimationShader.Destroy();
    s_UIShader.Destroy();
    s_SlotShader.Destroy();
    s_TextShader.Destroy();
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

    mesh.GetTexture()->Bind();
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.GetNumVertices());

    mesh.Unbind();

    Texture::Unbind();
    Texture3D::Unbind();
}

void Renderer::SubmitUI(const Mesh &mesh, const Vector2f &position, const Vector2i &size, int scale)
{
    if (!s_ActiveShader || !s_ActiveCamera)
        return;

    Matrix4 matrix(1.0f);
    matrix = glm::translate(matrix, Vector3(position.x, position.y, 0.0f));
    matrix = glm::scale(matrix, Vector3(size.x * scale, size.y * scale, 1.0f));

    Matrix4 projection = glm::ortho(0.0f, (float)s_MainWindow->GetWidth(), 0.0f, (float)s_MainWindow->GetHeight(), -1.0f, 1.0f);

    s_ActiveShader->SetUniform("u_Model", matrix);
    s_ActiveShader->SetUniform("u_View", Matrix4(1.0f));
    s_ActiveShader->SetUniform("u_Projection", projection);

    mesh.GetTexture()->Bind();
    mesh.Bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.GetNumVertices());

    mesh.Unbind();

    Texture::Unbind();
    Texture3D::Unbind();
}

void Renderer::SubmitText(const std::string &text, Font font, Vector2f position, float scale, const Vector3 &color)
{
    Renderer::Begin(s_TextShader);

    FontFace *face = FontManager::GetFont(font);
    if (!face)
        return;

    const auto &characters = face->GetCharacters();

    Matrix4 projection = glm::ortho(0.0f, (float)s_MainWindow->GetWidth(), 0.0f, (float)s_MainWindow->GetHeight(), -1.0f, 1.0f);

    s_ActiveShader->SetUniform("u_Projection", projection);

    for (char c : text)
    {
        auto it = characters.find(c);
        if (it == characters.end() && c != ' ')
            continue;

        const Character &ch = it->second;

        float xpos = position.x + ch.Bearing.x * scale;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Build 6 vertices for the quad
        std::vector<Vertex> verts = {
            {{xpos, ypos + h}, {0.0f, 0.0f}, 0},
            {{xpos, ypos}, {0.0f, 1.0f}, 0},
            {{xpos + w, ypos}, {1.0f, 1.0f}, 0},

            {{xpos, ypos + h}, {0.0f, 0.0f}, 0},
            {{xpos + w, ypos}, {1.0f, 1.0f}, 0},
            {{xpos + w, ypos + h}, {1.0f, 0.0f}, 0},
        };

        // Advance cursor
        position.x += (ch.Advance >> 6) * scale; // FreeType stores advance in 1/64 pixels

        if (c == ' ')
            continue;
        glActiveTexture(GL_TEXTURE0);
        // Bind the glyph texture
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update the dynamic mesh
        s_TextMesh->SetVertices(verts);

        s_TextShader.SetUniform("u_TextColor", color);
        s_TextShader.SetUniform("u_Texture", 0);

        s_TextMesh->Bind();

        // Draw quad
        glDrawArrays(GL_TRIANGLES, 0, s_TextMesh->GetNumVertices());
    }

    Mesh::Unbind();
    Texture::Unbind();
}

void Renderer::End()
{
    if (!s_ActiveShader)
        return;

    s_ActiveShader->Unbind();
}
