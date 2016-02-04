#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Mathematics.h"

#ifndef BOOM_H_
#define BOOM_H_

class Boom
{
	public:
		Boom();
		virtual ~Boom();

		void update(float delta);
		void draw();

		GLuint tex;
		Vec3 pos, bdir, dir, *camPos, *camFront;
		float color[3], fade, fadeDuration, fadeStart, z;
		float width, height;
		float life;
		float movementTime, lifeTime;
		float speed;
		bool active;
};

#endif /* BOOM_H_ */
