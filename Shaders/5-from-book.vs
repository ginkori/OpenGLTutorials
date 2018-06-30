#version 440 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 ourColor;
out vec2 texCoord;

layout (location = 4) uniform mat4 mv;
layout (location = 5) uniform mat4 projection;

void main()
{
    gl_Position = projection * mv * vec4(position.xyz, 1.0f);
    ourColor = color;
    texCoord = texture;
}