#include "scene.hpp"


using namespace cgp;



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
		{ 3.0f, -2.0f, 1.5f } /* position of the camera in the 3D scene */,
		{0,0,0} /* targeted point in 3D scene */,
		{0,0,1} /* direction of the "up" vector */);


	// General information
	display_info();

	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the shapes seen in the 3D scene
	// ********************************************** //
	
	// Create a mesh structure (here a cube)
	mesh cube_mesh = mesh_primitive_cube(/*center*/{ 0,0,0 }, /*edge length*/ 1.0f);
	// a mesh is simply a container of vertex data (position,normal,color,uv) and triangle index
	// the mesh data are stored on the CPU memory - they will need to be sent to the GPU memory before being drawn

	// Initialize a mesh drawable from a mesh structure
	//   - mesh : store buffer of data (vertices, indices, etc) on the CPU. The mesh structure is convenient to manipulate in the C++ code but cannot be displayed (data is not on GPU).
	//   - mesh_drawable : store VBO associated to elements on the GPU + associated uniform parameters. A mesh_drawable can be displayed using the function draw(mesh_drawable, environment). It only stores the indices of the buffers on the GPU - the buffer of data cannot be directly accessed in the C++ code through a mesh_drawable.
	//   Note: a mesh_drawable can be created from a mesh structure in calling [mesh_drawable_name].initialize_data_on_gpu([mesh_name])
	cube.initialize_data_on_gpu(cube_mesh);
	cube.material.color = { 1,1,0 };  // set the color of the cube (R,G,B) - sent as uniform parameter to the shader when display is called.
	cube.model.translation = { 1,1,0 }; // set the position of the cube - translation applied as uniform parameter to the "model matrix" in the shader when display is called.

	// Same process for the ground which is a plane 
	//  A quadrangle is defined a plane with 4-extremal corners.
	float L_ground = 3.0f;
	float z_ground = -0.51f;
	mesh ground_mesh = mesh_primitive_quadrangle(
		{ -L_ground, -L_ground, z_ground },
		{  L_ground, -L_ground, z_ground },
		{  L_ground,  L_ground, z_ground },
		{ -L_ground,  L_ground, z_ground });
	ground.initialize_data_on_gpu(ground_mesh);
	ground.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/checkboard.png");

	//Create a sphere structure
	mesh sphere_mesh = mesh_primitive_sphere(1.0f);
	sphere.initialize_data_on_gpu(sphere_mesh);

	sphere.model.scaling = 0.2f; //coordinates are multiplied by 0.2 in the shader
	sphere.model.translation = {1, 2, 0}; //coordinates are offseted by {1,2,0} in the shader
	sphere.material.color = {1, 0.5f, 0.5f}; //sphere will appear red (r,g,b components in [0,1])

	//Same procedure for camel mesh
	mesh camel_mesh = mesh_load_file_obj(project::path + "assets/camel.obj");
	camel.initialize_data_on_gpu(camel_mesh);
	camel.model.scaling = 0.5f; //adjust size
	camel.model.translation = {-1, 1, 0.5f}; //and position


	//Apply custom shaders to cube, sphere and camel
	opengl_shader_structure shader_custom;
	shader_custom.load(
		project::path + "shaders/mesh_custom/mesh_custom.vert.glsl",
		project::path + "shaders/mesh_custom/mesh_custom.frag.glsl");
	camel.shader = shader_custom;
	cube.shader = shader_custom;
	sphere.shader = shader_custom;

	std::cout << "End function scene_structure::initialize()" << std::endl;

	//possible to visualise and modify the projection matrix
	std::cout << str_pretty(camera_projection.matrix()) << std::endl;

}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{
	//to deactivate Zbuffer (curiosity only)
	//glDisable(GL_DEPTH_TEST);

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	// Update time
	timer.update();
	
	// the general syntax to display a mesh is:
	//   draw(mesh_drawableName, environment);
	// Note: scene is used to set the uniform parameters associated to the camera, light, etc. to the shader
	draw(ground, environment);
	draw(cube, environment);
	//draw sphere and camel
	draw(sphere, environment);
	draw(camel, environment);

	//to visualise wireframes
	if(gui.display_wireframe == true){
		draw_wireframe(ground, environment);
		draw_wireframe(sphere, environment);
		draw_wireframe(cube, environment);
		draw_wireframe(camel, environment, {1, 0, 0});//we can also set rgb color
	}


	// conditional display of the global frame (set via the GUI)
	if (gui.display_frame)
		draw(global_frame, environment);

	// we have a mesure of time in the simulation
	//std::cout << timer.t << std::endl;
	
	environment.uniform_generic.uniform_float["time"] = timer.t;

}

void scene_structure::display_gui()
{
	//checkbox controlling display of 3D reference axis
	ImGui::Checkbox("Frame", &gui.display_frame);
	
	//checkbox controlling display of objects' wireframes
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	//slider to control camel position
	ImGui::SliderFloat("camel-x", &camel.model.translation.x, -2.0f, 2.0f);

	//modify field of view angle (between 10 and 150 degrees)
	ImGui::SliderFloat("Field of view", &camera_projection.field_of_view, Pi/18.0f, 15*Pi/18.0f);
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
	std::cout << "Introductory scene for INF443." << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}

