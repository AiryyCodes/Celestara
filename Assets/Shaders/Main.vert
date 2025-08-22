#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in int a_Layer;

out vec2 v_UV;
flat out int v_Layer;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 0.0f, 1.0f);

    v_UV = a_UV;
    v_Layer = a_Layer;
}
