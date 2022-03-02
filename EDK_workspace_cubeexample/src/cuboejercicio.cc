#define CUBOS
#ifdef CUBOS


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
const float kdelta = matlib::kFloatPi * 2 / 300;

float sun_color[] = { 255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };

float mercury_color[] = { 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f };

float earth_color[] = { 255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f };
EDK3::ref_ptr<EDK3::MatDiffuse::Settings> mercury_material_settings;

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

	EDK3::ref_ptr<EDK3::MatDiffuse::Settings> earth_material_settings;

	diffuse_material.alloc();
	sun_material_settings.alloc();
	mercury_material_settings.alloc();
	earth_material_settings.alloc();
	// Nodes
	EDK3::Node *root = state->root.alloc();

	// Create a Drawable Node (Geometry + Material + Settings)
	EDK3::ref_ptr<EDK3::Drawable> drawable_sun;
	
	EDK3::ref_ptr<EDK3::Drawable> drawable_mercury;
	
	EDK3::ref_ptr<EDK3::Drawable> drawable_earth;

	// Graphic
	drawable_sun.alloc();

	drawable_mercury.alloc();
	
	
	drawable_earth.alloc();


	drawable_sun->set_geometry(cube_geometry.get());
	drawable_mercury->set_geometry(cube_geometry.get());
	drawable_earth->set_geometry(cube_geometry.get());

	drawable_sun->set_material(diffuse_material.get());
	drawable_mercury->set_material(diffuse_material.get());
	drawable_earth->set_material(diffuse_material.get());

	drawable_sun->set_material_settings(sun_material_settings.get());
	drawable_mercury->set_material_settings(mercury_material_settings.get());
	drawable_earth->set_material_settings(earth_material_settings.get());

	// Transform
	drawable_sun->set_position(-15.0f, 0.0f, -15.0f);
	drawable_sun->set_scale(1.0f, 1.0f, 1.0f);
	
	
	drawable_mercury->set_scale(0.5f, 0.5f, 0.5f);

	drawable_earth->set_scale(1.0f, 1.0f, 1.0f);


	
	sun_material_settings->set_color(sun_color);
	
	mercury_material_settings->set_color(mercury_color);
	
	earth_material_settings->set_color(earth_color);
	

	//root childs
	root->addChild(drawable_sun.get());
	root->addChild(drawable_mercury.get());
	root->addChild(drawable_earth.get());


	// Create a Camera
	state->flying_camera.alloc();
	state->flying_camera->Init();
	state->flying_camera->camera->setupPerspective(70, 1280.0f / 720.0f, 1.0f, 1500.0f);
	//state->flying_camera->camera->set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
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

void Update(GameState* state, float delta_time) {
	// Update

	static float iterator = 0;
	state->root->child(1)->set_position(
		cosf(kdelta * iterator) * 3 + state->root->child(0)->position()[0],
		sinf(kdelta * iterator) * 3 + state->root->child(0)->position()[1],
		+state->root->child(0)->position()[2]
	);
	float mouseX = (float)ESAT::MousePositionX() / (1280 / (16 * 2.10f));
	float mouseY = -(float)ESAT::MousePositionY() / (720 / (9 * 2.10f));

	mercury_color[2] = (
		(state->root->child(0)->position()[0] * state->root->child(1)->position()[0]) +
		(state->root->child(0)->position()[1] * state->root->child(1)->position()[1]) +
		(state->root->child(0)->position()[2] * state->root->child(1)->position()[2])
		);
	mercury_material_settings->set_color(mercury_color);

	state->root->child(2)->set_position(mouseX - 20, mouseY + 15, -15.0f);

	//Earth & orbit rotation
	iterator++;
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

		Update(&state, delta_time);
		
		render_function(&state);	
		ESAT::WindowFrame();
	}

	return 0;
}
#endif // CUBOS
