#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/matdiffuse.h>
#include <EDK3/dev/gpumanager.h>

#include "quad_geometry.h"

double previus_time;

struct GameState {
	EDK3::ref_ptr<EDK3::Camera> camera;
	EDK3::ref_ptr<EDK3::Node> root;	
};

void prepare(GameState *state) {
	EDK3::dev::GPUManager::CheckGLError("Prepare Start");
	
	// Create cube
	/*EDK3::ref_ptr<EDK3::Geometry> cube_geometry;
	EDK3::CreateCube(&cube_geometry, 1.0f, true, false);*/
	
	EDK3::ref_ptr<QuadGeometry> cube_geometry;
	cube_geometry.alloc();

	// Material
	EDK3::ref_ptr<EDK3::MatDiffuse> diffuse_material;
	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> diffuse_material_settings;

	diffuse_material.alloc();
	diffuse_material_settings.alloc();
	
	// Nodes
	EDK3::Node *root = state->root.alloc();

	// Create a Drawable Node (Geometry + Material + Settings)
	EDK3::ref_ptr<EDK3::Drawable> drawable;

	// Material
	diffuse_material_settings.alloc();

	// Graphic
	drawable.alloc();
	drawable->set_geometry(cube_geometry.get());
	drawable->set_material(diffuse_material.get());
	drawable->set_material_settings(diffuse_material_settings.get());

	// Transform
	drawable->set_position(0.0f, 0.0f, 0.0f);
	drawable->set_scale(0.5f, 0.5f, 0.5f);
	root->addChild(drawable.get());

	// Color 
	float material_color[] = { 255.0f / 255.0f, 40.0f / 255.0f, 76.0f / 255.0f, 1.0f };
	diffuse_material_settings->set_color(material_color);

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

void update_function(GameState *state) {
	

}

void render_function(GameState *state) {
	
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
		
		update_function(&state);
		render_function(&state);
		ESAT::WindowFrame();
	}

	return 0;
}
