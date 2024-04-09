#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// ***************************************** //
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 2.0f,-2.0f,1.0f }, { 0,0,0 });

	display_info();
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable body;
	mesh_drawable head;
	mesh_drawable beak;
	mesh_drawable eye_left;
	mesh_drawable eye_right;
	mesh_drawable wing_left;
	mesh_drawable wing_right;

	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	body.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); body.model.scaling_xyz = vec3{2.0f, 0.7f, 1.0f};
	head.initialize_data_on_gpu(mesh_primitive_sphere(0.7));
	beak.initialize_data_on_gpu(mesh_primitive_cone(0.35, 0.35, {0,0,0}, {1,0,0}));
	eye_left.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); eye_left.model.scaling_xyz = vec3{0.03, 0.1, 0.1};
	eye_right.initialize_data_on_gpu(mesh_primitive_sphere(1.0)); eye_right.model.scaling_xyz = vec3{0.03, 0.1, 0.1};
	wing_left.initialize_data_on_gpu(mesh_primitive_quadrangle({1,1,0}, {0,1,0}, {0,0,0}, {1,0,0})); wing_left.model.scaling_xyz = vec3{1.7, 1.7, 1.0};
	wing_right.initialize_data_on_gpu(mesh_primitive_quadrangle({1,1,0}, {0,1,0}, {0,0,0}, {1,0,0})); wing_right.model.scaling_xyz = vec3{1.7, 1.7, 1.0};

	// Set the color of some elements
	vec3 white = { 1.0f, 1.0f, 1.0f };
	body.material.color = white;
	head.material.color = white;
	wing_left.material.color = white;
	wing_right.material.color = white;

	vec3 orange = { 1.0f, 0.5f,  0.0f };
	beak.material.color = orange;

	vec3 black = {0, 0, 0};
	eye_left.material.color = black;
	eye_right.material.color = black;

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	hierarchy.add(body, "Body");
	hierarchy.add(head, "Head", "Body", {2.4, 0, 0});
	hierarchy.add(beak, "Beak", "Head", {0.55, 0, 0});
	hierarchy.add(eye_left, "Eye left", "Head", {0.6, -0.2, 0.3});
	hierarchy.add(eye_right, "Eye right", "Head", {0.6, 0.2, 0.3});
	hierarchy.add(wing_left, "Wing left", "Body", {-1.0f, -.5f, 0});
	hierarchy.add(wing_right, "Wing right", "Body", {-1.0f, .5f, 0});
}





void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	if (gui.display_frame)
		draw(global_frame, environment);

	// Update the current time
	timer.update();

	// Apply transformation to some elements of the hierarchy
	
	int aux = timer.t/M_PI;
	double oscillation = (aux%2 == 0) ? -M_PI/2.0 + timer.t : -M_PI/2 - timer.t;


	hierarchy["Wing left"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, -M_PI -oscillation);
	hierarchy["Wing right"].transform_local.rotation = rotation_transform::from_axis_angle({1, 0, 0}, oscillation);

	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);

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