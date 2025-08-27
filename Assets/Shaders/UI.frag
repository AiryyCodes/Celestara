#version 410 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2D u_Texture;

void main()
{
    v_FragColor = texture(u_Texture, v_UV);
}
