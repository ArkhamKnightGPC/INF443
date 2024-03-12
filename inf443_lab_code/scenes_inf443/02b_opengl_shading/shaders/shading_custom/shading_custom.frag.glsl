#version 330 core


// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// View matrix
uniform mat4 view;

struct material_structure
{
	vec3 color;  // Uniform color of the object
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform vec3 light_color;
uniform vec3 light_position;

//Phong parameters
uniform float diffuse;
uniform float specular;
uniform float exp_specular;
uniform vec3 fog_color;

void main()
{
    mat3 O = transpose(mat3(view)); // get the orientation matrix
    vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
    vec3 camera_position = -O * last_col;

	vec3 current_color;

    //AMBIENT LIGHT EFFECT
    vec3 ambiant_effect = ambiant * material.color * light_color;

    //DIFFUSION EFFECT
    vec3 dir_normal = normalize(fragment.normal);
    vec3 dir_light = normalize(light_position - fragment.position);
    float normal_dot_light = max(dot(dir_light, dir_normal), 0.0);

    vec3 diffuse_effect = diffuse * normal_dot_light * material.color * light_color;

    //SPECULAR REFLECTION EFFECT
    //we compute reflected vector (formula r = d - 2*dot(d,n)*n)
    vec3 ur = normalize(-dir_light - 2*(dot(-dir_light, dir_normal))*dir_normal);
    vec3 uv = normalize(camera_position);
    float aux = max(dot(ur, uv), 0.0);
    float exp_aux = pow(aux, exp_specular);

    vec3 specular_effect = specular * exp_aux * light_color;

    vec3 color_without_fog = ambiant_effect + diffuse_effect + specular_effect;

    //FOG EFFECT
    float dist_to_camera = length(camera_position - fragment.position);
    float max_dist = 15;
    float fog_coeff = min(dist_to_camera/max_dist, 1.0);

    current_color = (1 - fog_coeff)*color_without_fog + fog_coeff*fog_color;

    // Note: the last alpha component is not used here
	//FragColor = vec4(color_without_fog, 1.0); 	
    FragColor = vec4(current_color, 1.0);
}