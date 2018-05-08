#version 440 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 ourColor;
out vec2 texCoord;

uniform float xOffset;

void main()
{
    gl_Position = vec4(position.x + xOffset, position.y + xOffset, position.z + xOffset, 1.0f);
    ourColor = color;
    texCoord = texture;
}