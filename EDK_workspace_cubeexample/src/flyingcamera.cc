
#include "DarioElMejor/flyingcamera.h"
#include "ESAT/input.h"
#include <stdio.h> 

FlyingCamera::FlyingCamera() {

	scale_.x = 1.0f;
	scale_.y = 1.0f;
	scale_.z = 1.0f;
	speed_ = 0.55f;
	//mouse_pos_buffer
}

FlyingCamera::~FlyingCamera() {}

void FlyingCamera::UpdatePosition(float deltatime) {
	float speed = speed_;
	if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Shift)) {
		speed = speed * 2;
	}
	
	
	if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Right) || ESAT::IsKeyPressed('D')) {
		translate_.x += speed * deltatime;
	}

	if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Left) || ESAT::IsKeyPressed('A')) {
		translate_.x -= speed * deltatime;
	}

	if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Up) || ESAT::IsKeyPressed('W')) {
		translate_.y += speed * deltatime;
	}

	if (ESAT::IsSpecialKeyPressed(ESAT::kSpecialKey_Down) || ESAT::IsKeyPressed('S')) {
		translate_.y -= speed * deltatime;
	}
	
	if (ESAT::IsKeyPressed('L')) {
		rotate_.x -= speed/2 * deltatime;
	}
	if (ESAT::IsKeyPressed('J')) {
		rotate_.x += speed / 2 * deltatime;
	}
	if (ESAT::IsKeyPressed('I')) {
		rotate_.y += speed / 2 * deltatime;
	}
	if (ESAT::IsKeyPressed('K')) {
		rotate_.y -= speed / 2 * deltatime;
	}


	matlib::Matrix4x4 transform = matlib::Matrix4x4::GetTransform(
			translate_,
			scale_,
			rotate_.x,
			rotate_.y,
			rotate_.z
		);

	transform.Inverse();
	

	camera->set_view_matrix(transform.m);
	//camera->set_view_matrix(matlib::Matrix4x4::Identity().m);
}

void FlyingCamera::Init() {
	camera.alloc();

	matlib::Matrix4x4 transform = matlib::Matrix4x4::GetTransform(
		translate_,
		scale_,
		rotate_.x,
		rotate_.y,
		rotate_.z
	);

	transform.Inverse();

	camera->set_view_matrix(transform.m);
}

