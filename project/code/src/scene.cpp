#include "scene.hpp"

using namespace cgp;

void deform_terrain(mesh& m)
{
	// Set the terrain to have a gaussian shape
	for (int k = 0; k < m.position.size(); ++k)
	{
		vec3& p = m.position[k];
		float d2 = p.x*p.x + p.y * p.y;
		float z = exp(-d2 / 4)-1;

		z = z + 0.05f*noise_perlin({ p.x,p.y });

		p = { p.x, p.y, z };
	}

	m.normal_update();
}

// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize()
{
	std::cout << "Start function scene_structure::initialize()" << std::endl;

	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	camera_control.set_rotation_axis_z(); // camera rotates around z-axis
	//   look_at(camera_position, targeted_point, up_direction)
	camera_control.look_at(
		{ 5.0f, -4.0f, 3.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);

	// Display general information
	display_info();
	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Remove warnings for unset uniforms
	cgp_warning::max_warning = 0;
	// Load the custom shader
	opengl_shader_structure shader_custom;
	shader_custom.load(
		project::path + "shaders/shading_custom/shading_custom.vert.glsl",
		project::path + "shaders/shading_custom/shading_custom.frag.glsl");


	// Create the shapes seen in the 3D scene
	// ********************************************** //

	int N_terrain_samples = 100;
	float terrain_length = 50;
	mesh const terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/sand.jpg");
	terrain.material.phong.specular = 0.0f; // non-specular terrain material

	mesh pyramid_mesh = create_pyramid_mesh(10, 10, 0); pyramid_mesh.translate({10, 10, 0});
	mesh sphere_mesh = mesh_primitive_sphere(5.0f, {0,0,20}, 40, 20);

	pyramid.initialize_data_on_gpu(pyramid_mesh);
	pyramid.material.color = {204.0/256.0, 174.0/256.0, 135.0/256.0};
	//pyramid.material.phong.specular = 0.0f;
	//pyramid.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/pyramid.jpg");

	camel.initialize_data_on_gpu(mesh_load_file_obj(project::path + "assets/camel.obj"));
	camel.material.color = { 0.8, 0.7, 0.3 };
	camel.model.scaling = 1;
	camel.model.translation = { -4, 4, evaluate_terrain_height(-4, 4) + 1 };

	sphere_light.initialize_data_on_gpu(sphere_mesh);
	
	//pyramid.shader = shader_custom;
	camel.shader = shader_custom;
	terrain.shader = shader_custom;

	// Initialize SDL
    SDL_Init(SDL_INIT_AUDIO);

    // Load .wav file
    SDL_LoadWAV("../sounds/uncharted.wav", &wavSpec, &wavBuffer, &wavLength);

    // Open the audio device
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

    // Queue the audio buffer for playback
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);

    // Start playing audio
    SDL_PauseAudioDevice(deviceId, 0);

}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	// Update time
	timer.update();

	// conditional display of the global frame (set via the GUI)
	if (gui.display_frame)
		draw(global_frame, environment);
	

	// Draw all the shapes
	draw(terrain, environment);
	draw(pyramid, environment);
	draw(camel, environment);

	sphere_light.model.translation = gui.light_position;
	sphere_light.material.color = gui.light_color * 0.8f;
	sphere_light.material.phong.ambient = 1;
	sphere_light.material.phong.diffuse = 0;
	sphere_light.material.phong.specular = 0;
	draw(sphere_light, environment);

	if (gui.display_wireframe) {
		draw_wireframe(terrain, environment);
		draw_wireframe(pyramid, environment);
		draw_wireframe(camel, environment);
	}

	// Set additional uniform parameters to the shader
	environment.uniform_generic.uniform_float["ambiant"] = gui.ambiant;
	environment.uniform_generic.uniform_vec3["light_color"] = gui.light_color;
	environment.uniform_generic.uniform_vec3["light_position"] = gui.light_position;

	//add parameters for Phong Ilumination
	environment.uniform_generic.uniform_float["diffuse"] = gui.diffuse;
	environment.uniform_generic.uniform_float["specular"] = gui.specular;
	environment.uniform_generic.uniform_float["exp_specular"] = gui.exp_specular;
	environment.uniform_generic.uniform_vec3["fog_color"] = gui.fog_color;

	environment.background_color = gui.fog_color;
	
	// Wait for the sound to finish playing
    if (SDL_GetQueuedAudioSize(deviceId) == 0) {
        // and play it again!!!!
		SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceId, 0);
    }
    
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}


void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;


	std::cout << "\nSCENE INFO:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "Display here the information you would like to appear at the start of the program on the command line (file scene.cpp, function display_info())." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}