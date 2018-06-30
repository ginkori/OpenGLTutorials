#version 440 core
out vec4 o_FragColor;
  
layout (location = 7) uniform vec3 u_objectColor;
layout (location = 8) uniform vec3 u_lightColor;

in vec3 v_normal;
in vec3 v_pos;
in vec3 v_light_pos;

void main()
{
    	float ambientStrength = 0.1;
    	vec3 ambient = ambientStrength * u_lightColor;

	vec3 norm = normalize(v_normal);
	vec3 light_dir = normalize(v_light_pos - v_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * u_lightColor;
	
	vec3 view_dir = normalize(-v_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = spec * u_lightColor; 

    	vec3 result = (ambient + diffuse + specular) * u_objectColor;
    	o_FragColor = vec4(result , 1.0);
}