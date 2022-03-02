#define PLANETA
#ifdef PLANETAS


#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/matdiffuse.h>
#include <EDK3/dev/gpumanager.h>
#include "DarioElMejor/flyingcamera.h"


double previus_time;

struct GameState {
	EDK3::ref_ptr<FlyingCamera> flying_camera;
	EDK3::ref_ptr<EDK3::Node> root;
};

void prepare(GameState *state) {
	EDK3::dev::GPUManager::CheckGLError("Prepare Start");
	
	// Create cube
	EDK3::ref_ptr<EDK3::Geometry> cube_geometry;
	EDK3::CreateCube(&cube_geometry, 1.0f, true, false);
	
	// Material
	EDK3::ref_ptr<EDK3::MatDiffuse> diffuse_material;
	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> sun_material_settings;

	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> mercury_material_settings;
	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> venus_material_settings;
	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> earth_material_settings;

	diffuse_material.alloc();
	sun_material_settings.alloc();
	mercury_material_settings.alloc();
	venus_material_settings.alloc();
	earth_material_settings.alloc();
	// Nodes
	EDK3::Node *root = state->root.alloc();

	// Create a Drawable Node (Geometry + Material + Settings)
	EDK3::ref_ptr<EDK3::Drawable> drawable_sun;
	
	EDK3::ref_ptr<EDK3::Drawable> drawable_mercury;
	EDK3::ref_ptr<EDK3::Node> mercury_orbit;
	
	EDK3::ref_ptr<EDK3::Drawable> drawable_venus;
	EDK3::ref_ptr<EDK3::Node> venus_orbit;

	EDK3::ref_ptr<EDK3::Drawable> drawable_earth;
	EDK3::ref_ptr<EDK3::Node> earth_orbit;

	// Graphic
	drawable_sun.alloc();

	drawable_mercury.alloc();
	mercury_orbit.alloc();
	
	drawable_venus.alloc();
	venus_orbit.alloc();
	
	drawable_earth.alloc();
	earth_orbit.alloc();


	drawable_sun->set_geometry(cube_geometry.get());
	drawable_mercury->set_geometry(cube_geometry.get());
	drawable_venus->set_geometry(cube_geometry.get());
	drawable_earth->set_geometry(cube_geometry.get());

	drawable_sun->set_material(diffuse_material.get());
	drawable_mercury->set_material(diffuse_material.get());
	drawable_venus->set_material(diffuse_material.get());
	drawable_earth->set_material(diffuse_material.get());

	drawable_sun->set_material_settings(sun_material_settings.get());
	drawable_mercury->set_material_settings(mercury_material_settings.get());
	drawable_venus->set_material_settings(venus_material_settings.get());
	drawable_earth->set_material_settings(earth_material_settings.get());

	// Transform
	drawable_sun->set_position(0.0f, 0.0f, -5.0f);
	drawable_sun->set_scale(1.0f, 1.0f, 1.0f);
	
	
	drawable_mercury->set_position(1.0f, 0.0f, 0.0f);
	drawable_mercury->set_scale(0.1f, 0.1f, 0.1f);
	mercury_orbit->set_position(drawable_sun->position());

	drawable_venus->set_position(1.50f, 0.0f, 0.0f);
	drawable_venus->set_scale(0.25f, 0.25f, 0.25f);
	venus_orbit->set_position(drawable_sun->position());

	drawable_earth->set_position(2.30f, 0.0f, 0.0f);
	drawable_earth->set_scale(0.4f, 0.4f, 0.4f);
	earth_orbit->set_position(drawable_sun->position());

	float sun_color[] = { 245.0f / 255.0f, 170.0f / 255.0f, 50 / 255.0f, 1.0f };

	float mercury_color[] = { 145 / 255.0f, 132 / 255.0f, 80 / 255.0f, 1.0f };
	
	float venus_color[] = { 168 / 255.0f, 113 / 255.0f, 50 / 255.0f, 1.0f };
	
	float earth_color[] = { 66 / 255.0f, 123 / 255.0f, 245 / 255.0f, 1.0f };
	
	sun_material_settings->set_color(sun_color);
	
	mercury_material_settings->set_color(mercury_color);
	
	venus_material_settings->set_color(venus_color);

	earth_material_settings->set_color(earth_color);
	

	//root childs
	root->addChild(drawable_sun.get());
	root->addChild(mercury_orbit.get());
	root->addChild(venus_orbit.get());
	root->addChild(earth_orbit.get());
	mercury_orbit->addChild(drawable_mercury.get());
	venus_orbit->addChild(drawable_venus.get());
	earth_orbit->addChild(drawable_earth.get());


	// Create a Camera
	state->flying_camera.alloc();
	state->flying_camera->Init();
	state->flying_camera->camera->setupPerspective(70, 1280.0f / 720.0f, 1.0f, 1500.0f);
	state->flying_camera->camera->set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
	EDK3::dev::GPUManager::CheckGLError("Prepare END");
}

void render_function(GameState *state) {
	// For Every frame... determine what's visible:
	state->flying_camera->camera->doCull(state->root.get());

	// Render
	EDK3::dev::GPUManager::CheckGLError("begin Render-->");
	state->flying_camera->camera->doRender();
	EDK3::dev::GPUManager::CheckGLError("end Render-->");
}

void UpdatePosition(GameState* state, float delta_time) {
	// Update
	//Sun rotation
	state->root->child(0)->set_rotation_y(5.0f * ESAT::Time() / 1000.0f);

	//Mercury & orbit rotation
	state->root->child(1)->set_rotation_y(5.0f * ESAT::Time() / 250.0f);
	state->root->child(1)->child(0)->set_rotation_y(5.0f * ESAT::Time() / 150.0f);

	//Venus & orbit rotation
	state->root->child(2)->set_rotation_y(5.0f * ESAT::Time() / 200.0f);
	state->root->child(2)->child(0)->set_rotation_y(5.0f * ESAT::Time() / 200.0f);

	//Earth & orbit rotation
	state->root->child(3)->set_rotation_y(5.0f * ESAT::Time() / 300.0f);
	state->root->child(3)->child(0)->set_rotation_y(5.0f * ESAT::Time() / 100.0f);

	state->flying_camera->UpdatePosition(delta_time);
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

		UpdatePosition(&state, delta_time);
		
		render_function(&state);	
		ESAT::WindowFrame();
	}

	return 0;
}
#endif // PLANETAS
