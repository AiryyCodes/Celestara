#include "Graphics/Animation.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

void Animation::Update(float delta)
{
    m_Elapsed += delta;

    if (m_Elapsed >= m_Speed)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_NumFrames;
        m_Elapsed = 0.0f;
    }
}

void Animation::Render(const Spritesheet &sheet)
{
    const Shader *shader = Renderer::GetActiveShader();
    if (!shader)
        return;

    Vector2 sheetSize = Vector2(sheet.GetColumns(), sheet.GetRows());

    shader->SetUniform("u_SheetSize", sheetSize);
    shader->SetUniform("u_FrameIndex", m_CurrentFrame);
}
