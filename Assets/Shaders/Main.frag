#version 440 core

in vec2 v_UV;
in flat int v_Layer;

out vec4 v_FragColor;

uniform bool u_IsSamplerArray;

uniform sampler2D u_Texture;
uniform sampler2DArray u_TextureArray;

void main()
{
    if (u_IsSamplerArray)
    {
        v_FragColor = texture(u_TextureArray, vec3(v_UV, v_Layer));
    }
    else
    {
        v_FragColor = texture(u_Texture, v_UV);
    }
}
