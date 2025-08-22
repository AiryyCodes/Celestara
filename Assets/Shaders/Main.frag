#version 410 core

in vec2 v_UV;
flat in int v_Layer;

out vec4 v_FragColor;

uniform sampler2DArray u_TextureArray;

void main()
{
    v_FragColor = texture(u_TextureArray, vec3(v_UV, v_Layer));
}
