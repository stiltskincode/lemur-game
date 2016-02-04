#ifndef CAMERA_H_
#define CAMERA_H_

#include <GL/glew.h>
#include "EventManager.h"
#include "Object3D.h"
#include <SDL_OpenGL.h>
#include <SDL.h>

class Camera: public Object3D
{
	public:
		Camera();
		virtual ~Camera();

		virtual void yaw(float angle);
		virtual void pitch(float angle);
		virtual void roll(float angle);
		virtual void move(float f, float u, float r);
		virtual void useSDLEvents();
		virtual void draw();
		virtual void update();

		virtual void setSpeed(float s) {speed=s;}

	protected:
		float speed;
		bool active;
};

#endif /*CAMERA_H_*/
