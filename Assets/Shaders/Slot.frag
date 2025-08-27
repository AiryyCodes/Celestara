#version 410 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2DArray u_TextureArray;
uniform int u_Layer;

void main()
{
    v_FragColor = texture(u_TextureArray, vec3(v_UV, u_Layer));
}
