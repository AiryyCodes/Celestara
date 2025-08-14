#version 440 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;

out vec2 v_UV;

uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * vec4(a_Position, 0.0f, 1.0f);

    v_UV = a_UV;
}
