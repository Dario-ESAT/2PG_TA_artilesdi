#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/matdiffuse.h>
#include <EDK3/matcustom.h>
#include <EDK3/scoped_array.h>
#include <EDK3/dev/gpumanager.h>
#include <EDK3/dev/shader.h>
#include "supermaterial.h"


double previus_time;

struct GameState {
	EDK3::ref_ptr<EDK3::Camera> camera;
	EDK3::ref_ptr<EDK3::Node> root;
};

void prepare(GameState *state) {
	EDK3::dev::GPUManager::CheckGLError("Prepare Start");

	// Create spheres
	EDK3::ref_ptr<EDK3::Geometry> cube_geometry;
	EDK3::CreateCube(&cube_geometry, 1.0f, true, false);

	// Material
	EDK3::ref_ptr<SuperMaterial> custom_material;
	custom_material.alloc();
	custom_material->load_shaders_from_filepaths("./shaders/test.vert", "./shaders/test.frag");

	// Material Settings
	EDK3::ref_ptr<SuperMaterial::Settings> cutom_material_settings;
	cutom_material_settings.alloc();
	float green_color[] = { 130 / 255.0f, 171 / 255.0f, 70 / 255.0f, 1.0f };
	cutom_material_settings->set_color(green_color);

	// Nodes
	EDK3::Node *root = state->root.alloc();

	// Create a Drawable Node (Geometry+Material+Settings)
	EDK3::ref_ptr<EDK3::Drawable> drawable;

	// Graphic
	drawable.alloc();
	drawable->set_geometry(cube_geometry.get());
	drawable->set_material(custom_material.get());
	drawable->set_material_settings(cutom_material_settings.get());

	// Transform
	drawable->set_position(0.0f, 0.0f, 0.0f);
	drawable->set_scale(0.5f, 0.5f, 0.5f);
	root->addChild(drawable.get());


	// Create a Camera
	state->camera.alloc();
	float pos[] = { 0.0f, 0.0f, 1.5f };
	float view[] = { 0.0f, 0.0f, -1.0f };
	state->camera->set_position(pos);
	state->camera->set_view_direction(view);
	state->camera->setupPerspective(70, 1280.0f / 720.0f, 1.0f, 1500.0f);
	state->camera->set_clear_color(0.99f, 0.99f, 0.99f, 1.0f);
	EDK3::dev::GPUManager::CheckGLError("Prepare END");
}

void render_function(GameState *state) {

	// Update
	state->root->set_rotation_y(5.0f * ESAT::Time() / 300.0f);

	// For Every frame... determine what's visible:
	state->camera->doCull(state->root.get());

	// Render
	EDK3::dev::GPUManager::CheckGLError("begin Render-->");
	state->camera->doRender();
	EDK3::dev::GPUManager::CheckGLError("end Render-->");
}

int ESAT::main(int argc, char **argv) {

	// State
	GameState state;
	ESAT::WindowInit(1280, 720);
	ESAT::DrawSetTextFont("test.ttf");
	ESAT::DrawSetTextSize(18);
	ESAT::DrawSetFillColor(253, 255, 255, 128);
	prepare(&state);

	// Main loop
	while (ESAT::WindowIsOpened() && !ESAT::IsSpecialKeyDown(ESAT::kSpecialKey_Escape)) {

		double currentTime = RawTime();
		double delta_time = (currentTime - previus_time) / 1000.0f;
		previus_time = currentTime;

		render_function(&state);
		ESAT::DrawBegin();
		ESAT::DrawText(10, 20, "cube example");
		ESAT::DrawEnd();
		ESAT::WindowFrame();
	}

	return 0;
}
