#version 330

in vec3 v_normal;
in vec3 v_position;
in vec3 v_eye;

layout(location = 0) out vec4 o_color;

void main()
{
	const vec3 light_pos = vec3(0.0f, 5.0f, 0.0f);
	vec3 light_vec = normalize(v_position - light_pos);
	vec3 norm_vec = normalize(v_normal);
	float d_angle = dot(-light_vec, norm_vec);
	float diffuse = max(d_angle, 0.1f);

	vec3 refl_vec = normalize(reflect(light_vec, norm_vec));
	float s_angle = max(dot(refl_vec, v_eye), 0.0);
	float specular = pow(s_angle,5);

	o_color = vec4(vec3(1.0, 0.0, 0.0) * diffuse + vec3(specular), 1.0);
}