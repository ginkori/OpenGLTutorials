#version 440 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

layout (binding = 0) uniform sampler2D ourTexture0;
layout (binding = 1) uniform sampler2D ourTexture1;

layout (location = 6) uniform  float mixValue;

void main()
{
    color = mix(texture(ourTexture0, texCoord), texture(ourTexture1, vec2(texCoord.x, texCoord.y)), mixValue);// * vec4(ourColor, 1.0);
}