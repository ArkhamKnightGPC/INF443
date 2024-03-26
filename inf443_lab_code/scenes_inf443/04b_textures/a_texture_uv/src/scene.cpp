#include "scene.hpp"

#include "models_textures.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.look_at({ 3.0f, 2.0f, 2.0f }, {0,0,0}, {0,0,1});
	

	display_info();

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Geometry creation
	//-----------------------------------
	// Create a quadrangle as a mesh
	mesh quadrangle_mesh;
	quadrangle_mesh.position = { {-1,-1,0}, { 1,-1,0}, { 1, 1,0}, {-1, 1,0} };

	// Associate Texture-Coordinates to the vertices of the quadrangle
	//quadrangle_mesh.uv = { {0,0}, {1,0}, {1,1}, {0,1} }; 

	//EXPERIMENTS
	//quadrangle_mesh.uv   = { {0,0}, {0.5f,0}, {0.5f,0.5f}, {0,0.5f} };
	//quadrangle_mesh.uv   = { {0.25,0.25}, {0.5,0.25}, {0.5,0.5}, {0.25,0.5} };
	quadrangle_mesh.uv   = { {0,0}, {2,0}, {2,2}, {0,2} };
	//quadrangle_mesh.uv   = { {0,0}, {1,0}, {1,3}, {0,3} };
	//quadrangle_mesh.uv   = { {-1,-1}, {2,-1}, {2,2}, {-1,2} };

	quadrangle_mesh.connectivity = { {0,1,2}, {0,2,3} };

	quadrangle_mesh.fill_empty_field(); // (fill with some default values the other buffers (colors, normals) that we didn't filled before)


	// Convert the mesh structure into a mesh_drawable structure
	quad_shape.initialize_data_on_gpu(quadrangle_mesh);

	mesh torus_mesh = torus_with_texture();
	torus_shape.initialize_data_on_gpu(torus_mesh);

	mesh tree_mesh = cylinder_with_texture();
	cylinder_shape.initialize_data_on_gpu(tree_mesh);

	mesh disc_mesh = disc_with_texture();
	disc_shape.initialize_data_on_gpu(disc_mesh);


	// Texture Image load and association
	//-----------------------------------	

	// Load an image from a file, and send the result to the GPU
	quad_shape.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/squirrel.jpg",
		GL_REPEAT, //alternatives: GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT
		GL_REPEAT)
		;

	torus_shape.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/smiley.png",
		GL_REPEAT,
		GL_REPEAT);

	cylinder_shape.texture.load_and_initialize_texture_2d_on_gpu(
		project::path + "assets/trunk.jpg",
		GL_REPEAT,
		GL_REPEAT);

	disc_shape.texture.load_and_initialize_texture_2d_on_gpu(
		project::path + "assets/tree-ring.png",
		GL_CLAMP_TO_BORDER,
		GL_CLAMP_TO_BORDER);
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	//draw(shape, environment);
	quad_shape.model.translation = vec3{0, 0, 0};
	draw(quad_shape, environment);

	torus_shape.model.translation = vec3{-4, 0, 0};
	draw(torus_shape, environment);

	cylinder_shape.model.translation = vec3{3, 0, 0};
	draw(cylinder_shape, environment);

	disc_shape.model.translation = vec3{3, 0, 2};
	draw(disc_shape, environment);
	disc_shape.model.translation = vec3{3, 0, -2};
	draw(disc_shape, environment);

	if(gui.display_wireframe){
		draw_wireframe(quad_shape, environment, { 1,0,0 });
		draw_wireframe(torus_shape, environment, { 1,0,0 });
		draw_wireframe(cylinder_shape, environment, { 1,0,0 });
		//draw_wireframe(disc_shape, environment, { 1,0,0 });
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
}

