#version 440 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_TexCoords;

layout (location = 3) uniform vec3 u_lightPos;
layout (location = 4) uniform mat4 u_model;
layout (location = 5) uniform mat4 u_view;
layout (location = 6) uniform mat4 u_projection;

out vec3 v_normal;
out vec3 v_pos;
out vec3 v_light_pos;
out vec2 v_TexCoords;

void main()
{
	v_pos = vec3(u_view * u_model * vec4(a_pos, 1.0));
	// Send it from uniform in future
	v_normal = mat3(transpose(inverse(u_view * u_model))) * a_normal;
	v_light_pos = vec3(u_view * vec4(u_lightPos, 1.0));
	v_TexCoords = a_TexCoords;
	gl_Position = u_projection * vec4(v_pos, 1.0);
}