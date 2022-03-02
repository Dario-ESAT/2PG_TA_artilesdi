#ifndef INCLUDE_FLYINGCAMERA_H_
#define INCLUDE_FLYINGCAMERA_H_ 1

#include "EDK3/ref_ptr.h"
#include "EDK3/camera.h"
#include "mathlib/mathlib.h"

class FlyingCamera : public virtual EDK3::Referenced {
	public:
	FlyingCamera();
	~FlyingCamera();

	void UpdatePosition(float deltatime);
	void Init();

	EDK3::ref_ptr<EDK3::Camera> camera;
	float speed_;
	matlib::Vector3 translate_;
	matlib::Vector3 scale_;
	matlib::Vector3 rotate_;

	private:
		matlib::Vector2 mouse_pos_buffer;

};

#endif
