#pragma once

#include "Math/Transform.h"

class WorldObject
{
public:
    virtual void Init() {}
    virtual void Update(float delta) {}
    virtual void Render() {}

    Transform &GetTransform() { return m_Transform; }
    const Transform &GetTransform() const { return m_Transform; }

private:
    Transform m_Transform;
};
