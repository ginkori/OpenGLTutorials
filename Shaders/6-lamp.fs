#version 440 core

layout (location = 6) uniform vec3 u_lightColor;

out vec4 o_FragColor;

void main()
{
    o_FragColor = vec4(u_lightColor, 1.0);
}