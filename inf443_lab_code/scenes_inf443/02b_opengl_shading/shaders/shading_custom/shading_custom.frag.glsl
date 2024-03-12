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
uniform float diffuse;
uniform vec3 light_color;
uniform vec3 light_position;

void main()
{
	vec3 current_color;
	//current_color = ambiant * material.color * light_color;

    //Illumination de Phong
    vec3 dir_normal = normalize(fragment.normal);
    vec3 dir_light = normalize(light_position - fragment.position);

    float normal_dot_light = max(dot(dir_light, dir_normal), 0.0);

    float coefficient = ambiant + diffuse*normal_dot_light;

    current_color = coefficient * material.color * light_color;

	FragColor = vec4(current_color, 1.0); 	// Note: the last alpha component is not used here
}