#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Mathematics.h"
//#include "ParticleEmitter.h"
#include "Camera.h"
#include "TGAImage.h"
#include <string>

class Particle
{
	friend class ParticleEmitter;

	public:

		Particle();
		virtual ~Particle();

		void init(float *d, Camera *c);
		bool create(bool active, std::string texture, float life, float width, float height, Vec3 pos, Vec3 dir, Vec3 bdir, float speed, float fadeStart, float fadeDiration);
		bool update();
		bool draw();

	//private:
		float *delta;
		Camera *camera;
		GLuint tex;

	//public:
		Vec3 sumDir, pos, bdir, dir;
		float fade, fadeDuration, fadeStart, var;
		float width, height;
		float movementTime, lifeTime, life;
		float speed;
		bool active;

		bool initialized;
};

#endif
