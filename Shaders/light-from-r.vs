#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_mv;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_eye;

void main()
{
	vec4 pos = vec4(a_position, 1.0);
	v_position = vec3(u_mv * pos);
	v_normal = transpose(inverse(mat3(u_mv))) * a_normal;
	v_eye = normalize(vec3(0.0) - v_position);
	gl_Position = u_mvp * pos;
}