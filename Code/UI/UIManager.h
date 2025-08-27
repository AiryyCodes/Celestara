#pragma once

#include "Memory.h"
#include "Renderer/Mesh.h"
#include "UI/Element.h"

#include <utility>

class UIManager
{
public:
    void Init();
    void Update();
    void Render();

    template <typename T, typename... Args>
    void SetElement(int zIndex, const Args &&...args)
    {
        Ref<T> element = CreateRef<T>(std::move(args)...);
        m_ActiveElement = element;
    }
    void SetElement(const Ref<UIElement> &element) { m_ActiveElement = element; }

    const Ref<UIElement> &GetActiveElement() const { return m_ActiveElement; }

private:
    Mesh m_Mesh;

    Ref<UIElement> m_ActiveElement;
};
