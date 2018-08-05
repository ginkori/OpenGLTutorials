#version 440 core
 
struct Material {
    float shininess;
}; 
layout (binding = 0) uniform sampler2D u_diffuse;
layout (binding = 1) uniform sampler2D u_specular;
layout (binding = 2) uniform sampler2D u_emission;

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
in vec2 v_TexCoords;

out vec4 o_FragColor;

void main()
{
    vec3 ambient = u_light.ambient * vec3(texture(u_diffuse, v_TexCoords));

	vec3 norm = normalize(v_normal);
	vec3 light_dir = normalize(v_light_pos - v_pos);
    //vec3 light_dir = normalize(-u_light.direction);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_diffuse, v_TexCoords)); 
	
	vec3 view_dir = normalize(-v_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), u_material.shininess);
	vec3 specular = u_light.specular * spec * vec3(texture(u_specular, v_TexCoords)); 

    vec3 emission = texture(u_emission, v_TexCoords).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    o_FragColor = vec4(result , 1.0);
}