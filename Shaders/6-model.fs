#version 440 core
 
 struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_light;  
uniform Material u_material; 

in vec3 v_normal;
in vec3 v_pos;
in vec3 v_light_pos;

out vec4 o_FragColor;

void main()
{
    vec3 ambient = u_light.ambient * u_material.ambient;

	vec3 norm = normalize(v_normal);
	vec3 light_dir = normalize(v_light_pos - v_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);
	
	vec3 view_dir = normalize(-v_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * (spec * u_material.specular); 

    vec3 result = ambient + diffuse + specular;
    o_FragColor = vec4(result , 1.0);
}