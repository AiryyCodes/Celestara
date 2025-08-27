#pragma once

#include "Math/Math.h"
#include "Renderer/Texture.h"

#include <string>

class UIElement
{
public:
    UIElement(const Vector2i &size, const std::string &texPath)
        : m_Size(size)
    {
        m_Texture.SetTexture(texPath);
    }
    UIElement(const std::string &texPath)
    {
        m_Texture.SetTexture(texPath);
    }

    virtual ~UIElement() = default;

    virtual void Render() {}

    virtual void OnClick(int x, int y) {}
    virtual void OnWindowResize(int width, int height) {};

    bool IsInside(int mouseX, int mouseY) const;

    const Vector2f &GetPosition() const { return m_Position; }
    void SetPosition(const Vector2f &position) { m_Position = position; }

    const Vector2i &GetSize() const { return m_Size; }
    void SetSize(const Vector2i &size) { m_Size = size; }

    int GetScale() const { return m_Scale; }
    void SetScale(int scale) { m_Scale = scale; }

    const Texture &GetTexture() const { return m_Texture; }

protected:
    void Center();

private:
    Vector2f m_Position = Vector2f(0.0f, 0.0f);
    Vector2i m_Size = Vector2i(32, 32);
    int m_Scale = 1;

    Texture m_Texture;
};
