#pragma once

#include "Graphics/Spritesheet.h"

class Animation
{
public:
    Animation(int numFrames)
        : m_NumFrames(numFrames) {}
    Animation(int numFrames, float speed)
        : m_NumFrames(numFrames), m_Speed(speed) {}

    void Update(float delta);
    void Render(const Spritesheet &sheet);

    void Reset();

private:
    int m_NumFrames = 0;
    float m_Speed = 1.0f;

    int m_CurrentFrame = 0;
    float m_Elapsed = 0.0f;
};
