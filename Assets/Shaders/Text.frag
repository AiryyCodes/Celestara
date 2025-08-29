#version 410 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2D u_Texture;
uniform vec3 u_TextColor;
uniform float u_Threshold = 0.3;

void main()
{
    float distance = texture(u_Texture, v_UV).r;
    float alpha = smoothstep(u_Threshold - 0.01, u_Threshold + 0.01, distance);
    v_FragColor = vec4(u_TextColor, alpha);
}

