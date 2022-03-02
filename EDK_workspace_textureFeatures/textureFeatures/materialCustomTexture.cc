#include <ESAT/window.h>
#include <ESAT/input.h>
#include <ESAT/draw.h>
#include <ESAT/time.h>

#include <EDK3/geometry.h>
#include <EDK3/camera.h>
#include <EDK3/drawable.h>
#include <EDK3/matcustomtexture.h>
#include <EDK3/texture.h>
#include <EDK3/dev/gpumanager.h>

#include <iostream>
#include <cmath>
#include "math.h"

double previus_time;

using namespace EDK3;

struct GameState {
	ref_ptr<Camera> camera;
	ref_ptr<Node> root;
};

void prepare(GameState *state) {
	dev::GPUManager::CheckGLError("Prepare Start");

	// Create cube
	ref_ptr<Geometry> cubeGeometry;
	CreateCube(&cubeGeometry, 1.0f, true, true);

	// Material
	ref_ptr<MatCustomTexture> texture_material;
	ref_ptr<MatCustomTexture::Settings> texture_material_settings;

	texture_material.alloc();
	texture_material_settings.alloc();

	// Nodes
	Node *root = state->root.alloc();

	// Create a Drawable Node (Geometry+Material+Settings)
	ref_ptr<Drawable> drawable;

	// Material
	texture_material_settings.alloc();

	
	// Graphic
	drawable.alloc();
	drawable->set_geometry(cubeGeometry.get());
	drawable->set_material(texture_material.get());
	drawable->set_material_settings(texture_material_settings.get());

	// Transform
	drawable->set_position(0.0f, 0.0f, 0.0f);
	drawable->set_scale(0.5f, 0.5f, 0.5f);
	root->addChild(drawable.get());

	// Texture
	ref_ptr<Texture> texture;
	Texture::Load("blockguy.png", &texture);
	//Ejercicio 2 --------
	texture->set_mag_filter(EDK3::Texture::F_NEAREST);
	//--------------------
	//Ejercicio 3 --------
	//texture->set_wrap(EDK3::Texture::W_CLAMP_TO_EDGE);
	//--------------------

	// Settings
	texture_material_settings->set_texture(texture.get());
	float customData[] { 1.0f, 0.0f, 0.0f, 0.0f };
	texture_material_settings->set_custom_data(customData);

	// Create a Camera
	state->camera.alloc();
	float pos[] = { 0.0f,0.0f, 2.0f};
	float view[] = { -0.0f,-0.0f,-1.0f };
	state->camera->set_position(pos);
	state->camera->set_view_direction(view);
	state->camera->setupPerspective(70, 1280.0f / 720.0f, 1.0f, 1500.0f);
	state->camera->set_clear_color(0.99f, 0.99f, 0.99f, 1.0f);
	dev::GPUManager::CheckGLError("Prepare END");
}

void render_function(GameState *state) {
	Drawable* figura = (Drawable*)state->root->child(0);
	MatCustomTexture::Settings* material = (MatCustomTexture::Settings*) figura->material_settings();
	float customData[]{ ESAT::Time(), 0.0f, 0.0f, 0.0f };
	material->set_custom_data(customData);
	// For Every frame... determine what's visible:
	state->camera->doCull(state->root.get());

	// Render
	dev::GPUManager::CheckGLError("begin Render-->");
	state->camera->doRender();
	dev::GPUManager::CheckGLError("end Render-->");

	
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
		ESAT::DrawText(10, 20, "EDK Playground: Texture Features");
		ESAT::DrawEnd();
		ESAT::WindowFrame();
	}

	return 0;
}
