#version 410 core

in vec2 v_UV;

out vec4 v_FragColor;

uniform sampler2D u_Texture;
uniform vec3 u_TextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Texture, v_UV).r);
    v_FragColor = vec4(u_TextColor, 1.0) * sampled;
}  
