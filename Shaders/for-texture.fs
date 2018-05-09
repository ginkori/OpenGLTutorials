#version 440 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform  float mixValue;

void main()
{
    color = mix(texture(ourTexture1, texCoord), texture(ourTexture2, vec2(1.0 - texCoord.x, texCoord.y)), mixValue) * vec4(ourColor, 1.0);
}