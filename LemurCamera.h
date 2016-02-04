#pragma once

#include "Camera.h"

class LemurCamera : public Camera
{
	public:
		LemurCamera();
		~LemurCamera();

		virtual void init(Vec3 *characterFront, Vec3 *characterPosition, float *rotationAngle);
		virtual void update();

		Vec3 *characterFront,  *characterPosition, lastCharacterPosition;
		float *rotationAngle;
		float angleLeft;
		float distVar, distVarLimit;
};
