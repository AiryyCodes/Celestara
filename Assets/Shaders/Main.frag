#version 440 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2D u_Texture;

void main()
{
    v_FragColor = vec4(0.9f, 0.5f, 0.3f, 1.0f);

    v_FragColor = texture(u_Texture, v_UV);
}
