#version 440 core
layout (location = 0) in vec3 a_Pos;

layout (location = 7) uniform mat4 u_model;
layout (location = 8) uniform mat4 u_view;
layout (location = 9) uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_Pos, 1.0);
}